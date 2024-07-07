#pragma once

#include "net_common.hpp"
#include "net_tsqueue.hpp"
#include "net_message.hpp"

namespace net {
	template <typename T>
	class connection : public std::enable_shared_from_this<connection<T>> {
	public:
		time_t pingTime = -1;

		int connectionID;

		enum class owner {
			server,
			client
		};

		connection(owner root, asio::io_context& asioContext, asio::ip::tcp::socket socket, tsqueue<owned_message<T>>& qIn)
			:	m_asioContext(asioContext),
				m_socket(std::move(socket)),
				m_qMessagesIn(qIn),
				connectionID(-1)
		{
			m_OwnerType = root;
		}

		virtual ~connection() {};

		public:
			void ConnectToClient(int nid = 0) {
				if (m_OwnerType == owner::server) {
					if (m_socket.is_open()) {
						connectionID = nid;
						ReadHeader();
					}
				}
			}

			void ConnectToServer(const asio::ip::tcp::resolver::results_type &endpoints) {

				//only client can connect to server. could try some mesh network thing otherwise
				if (m_OwnerType == owner::client) {
					asio::async_connect(m_socket, endpoints,
						[this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
							if (!ec) {
								ReadHeader();
							}
						});
				}
			}

			bool Disconnect() {
				if (isConnected()) {
					asio::post(m_asioContext, [this]() {m_socket.close(); });
					return true;
				}
				return false;
			}

			bool isConnected() const {
				return m_socket.is_open();
			}

			void Send(const message<T>& msg) {
				asio::post(m_asioContext, 
					[this, msg]() {
						bool isQueueWorking = !m_qMessagesOut.empty();

						m_qMessagesOut.push_back(msg);

						if (!isQueueWorking)
							WriteHeader();
					});
			}


			std::string ToString() {
				return std::string("[") + GetIP() + ":" + std::to_string(GetPort()) + "]";
			}

			std::string GetIP() {
				return m_socket.local_endpoint().address().to_string();
			}

			asio::ip::port_type GetPort() {
				return m_socket.local_endpoint().port();
			}

		//asio
		protected:
			asio::ip::tcp::socket m_socket; //unique remote socket
			
			asio::io_context& m_asioContext;//singleton

			//outgoing messages
			tsqueue<message<T>> m_qMessagesOut;

			//messages received from remote
			tsqueue<owned_message<T>>& m_qMessagesIn;

			//internal message management io buffers
			message<T> m_msgBuffIn;

			owner m_OwnerType = owner::server;

		private:
			//ASYNC - prime context ready to read message header
			void WriteHeader() {
				asio::async_write(m_socket, asio::buffer(&m_qMessagesOut.front().header, sizeof(message_header<T>)),
					[this](std::error_code ec, std::size_t length) {
						if (ec) {
							m_socket.close();
							return;
						}

						if (m_qMessagesOut.front().body.size() > 0) {
							WriteBody();
						} else {
							m_qMessagesOut.pop_front();

							if (!m_qMessagesOut.empty())
								WriteHeader();
						}
					});
			}

			//ASYNC - prime context to write a message header
			void ReadHeader() {
				asio::async_read(m_socket, asio::buffer(&m_msgBuffIn.header, sizeof(message_header<T>)),
					[this](std::error_code ec, std::size_t length) {
						if (ec) {
							m_socket.close();
							return;
						}

						m_msgBuffIn.body.resize(m_msgBuffIn.header.size);
						if (m_msgBuffIn.header.size > 0) {
							ReadBody();
						} else {
							AddToIncomingMessageQueue();
						}
					});
			}

			//ASYNC - prime context ready to write message body
			void WriteBody() {
				asio::async_write(m_socket, asio::buffer(m_qMessagesOut.front().body.data(), m_qMessagesOut.front().body.size()),
					[this](std::error_code ec, std::size_t length) {
						if (ec) {
							m_socket.close();
							return;
						}

						m_qMessagesOut.pop_front();

						if (!m_qMessagesOut.empty()) {
							WriteHeader();
						}
					});
			}

			//ASYNC - prime context to read a message body
			void ReadBody() {
				asio::async_read(m_socket, asio::buffer(m_msgBuffIn.body.data(), m_msgBuffIn.body.size()),
					[this](std::error_code ec, std::size_t length) {
						if (ec) {
							m_socket.close();
							return;
						}

						AddToIncomingMessageQueue();
					});
			}

			void AddToIncomingMessageQueue() {
				if (m_OwnerType == owner::server)
					m_qMessagesIn.push_back({this->shared_from_this(), m_msgBuffIn});
				else
					m_qMessagesIn.push_back({ nullptr, m_msgBuffIn });

				//reprime asio
				ReadHeader();
			}
	};
}