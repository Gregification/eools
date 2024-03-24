#pragma once

#include "net_common.hpp"
#include "net_tsqueue.hpp"
#include "net_message.hpp"
#include "net_conneciton.hpp"

namespace net {

	/*
	* responsible for setting up connection and asio.
	* acts as client to server interface
	*/
	template <typename T>
	class client_interface {
		friend class client_interface;

		public:
			client_interface() {

			}
			
			~client_interface() {
				Disconnect();
			}

		public:
			bool Connect(const std::string& host, const uint16_t port) {
				try { //establish asio connection
					//resolve host string to something asio can target
					asio::ip::tcp::resolver resolver(m_context);
					asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(host, std::to_string(port)); //throws exception on bad input

					m_connection = std::make_unique<connection<T>>(
						connection<T>::owner::client,
						m_context,
						asio::ip::tcp::socket(m_context),
						m_qMessagesIn
					);
					
					m_connection->ConnectToServer(endpoint);

					thrContext = std::thread([this]() {m_context.run(); });
				} catch (std::exception&) { //intended for asio exceptions
					return false;
				}

				return false;
			}

			void Disconnect() {
				if (isConnected())
					m_connection->Disconnect();

				//asio shutdown
				m_context.stop();

				if (thrContext.joinable())
					thrContext.join();

				m_connection.release();
			}

			bool isConnected() {
				if(m_connection)
					return m_connection->isConnected();
				return false;
			}

			void Send(const message<T>& msg) {
				if (isConnected())
					m_connection->Send(msg);
			}

			tsqueue<owned_message<T>>& Incomming() {
				return m_qMessagesIn;
			}

			size_t Update(size_t nMaxMessages = -1) {
				size_t nMessageCount = 0;
				while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty()) {
					auto msg = m_qMessagesIn.pop_front();

					OnMessage(msg.msg);

					nMessageCount++;
				}

				return nMessageCount;
			}

		protected:
			/*asio context used*/
			asio::io_context m_context;
			/*thread running the asio context*/
			std::thread thrContext;

			/*connected instance of socket, handles data transfer*/
			std::unique_ptr<connection<T>> m_connection;

			/*messages from server*/
			tsqueue<owned_message<T>> m_qMessagesIn;

			virtual void OnMessage(message<T>& msg) {

			}

	};
}