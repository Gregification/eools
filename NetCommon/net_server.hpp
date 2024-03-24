#pragma once

#include "net_common.hpp"
#include "net_tsqueue.hpp"
#include "net_message.hpp"
#include "net_conneciton.hpp"

namespace net {
	template <typename T>
	class server_interface {
		public:
			server_interface(uint16_t listeningPort)
				: listeningPort(listeningPort), m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), listeningPort)) 
			{

			}

			virtual ~server_interface() {
				Stop();
			}

			bool Start() {
				try {
					WaitForClientConnection();//init first task. context will otherwise immideatly exit

					m_threadContext = std::thread([this]() {m_asioContext.run(); });
				} catch (std::exception& e) {
					onError(std::string("[SERVER] Exception: ") + e.what());
					return false;
				}

				onEvent("[SERVER] started");

				return true;
			}

			void Stop() {
				m_asioContext.stop();

				if (m_threadContext.joinable())
					m_threadContext.join();
			}

			/*ASYNC - instruct asio to wait for connection*/
			void WaitForClientConnection() {
				m_asioAcceptor.async_accept(
					[this](std::error_code ec, asio::ip::tcp::socket socket) {
						if (ec) {
							onError(std::string("New Connection Error: ") + ec.message());
						}
						else {

							onEvent(std::string("connection request [") + std::string(socket.local_endpoint().address().to_string()) + ":" + std::to_string(socket.local_endpoint().port()) + "]");

							std::shared_ptr<connection<T>> newconn =
								std::make_shared<connection<T>>(connection<T>::owner::server,
									m_asioContext, std::move(socket), m_qMessagedIn);

							//filter connection
							if (onClientConnect(newconn)) { //if connection approved
								std::lock_guard lk(m_mutexDeqConnections);
								m_deqConnections.push_back(std::move(newconn));

								m_deqConnections.back()->ConnectToClient(nIDCounter++);

								onEvent("connection accepted [" + std::to_string(m_deqConnections.back()->GetID()) + "]");
							}
							else {
								onEvent("connection denied");
							}
						}
						//reprime asio context 
						WaitForClientConnection();
					});
			}

			//send message to specific client
			void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg) {
				if (client && client->isConnected()) {
					client->Send(msg);
				} else {
					onClientDisconnect(client);
					client.reset();

					std::lock_guard lk(m_mutexDeqConnections);

					m_deqConnections.erase(
						std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
				}
			}

			//broadcast
			void MessageAllClients(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr) {
				bool invalidClientExists = false;
				std::lock_guard lk(m_mutexDeqConnections);

				for (auto& client : m_deqConnections) {

					if (client && client->isConnected()) {

						if (client != pIgnoreClient)
							client->Send(msg);
						else { //contact failed, assume client is disconnected
							invalidClientExists = true;

							onClientDisconnect(client);
							client.reset();
						}
					}
				}

				if (invalidClientExists) {
					m_deqConnections.erase(
						std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr), m_deqConnections.end());
				}
			}

			//trigger to start processing given number of messages. defaults to max count. calls this.OnMessage for each
			bool Update(size_t nMaxMessages = -1) {
				size_t nMessageCount = 0;
				while (nMessageCount < nMaxMessages && !m_qMessagedIn.empty()) {
					auto msg = m_qMessagedIn.pop_front();

					OnMessage(msg.remote, msg.msg);

					nMessageCount++;
				}

				return nMessageCount > 0;
			}

		protected:
			//returns true/accept or false/decline regarding the connection. think Swing predicate filters
			virtual bool onClientConnect(std::shared_ptr<connection<T>> client) {
				return false;
			}

			virtual void onClientDisconnect(std::shared_ptr<connection<T>> client) {

			}

			//on message from specific given client
			virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg) {

			}

			//for user purposes, good luck tring to use this for anyhting otherwise. rip
			virtual void onError(std::string message) {
				std::cerr << message;
			}
			
			//on notable evets such as connection being denied, user joining and so on
			virtual void onEvent(std::string message) {
				std::cerr << message << "\a\n";
			}

		protected:
			//incomming
			tsqueue<owned_message<T>> m_qMessagedIn;

			//active validated connections
			std::recursive_mutex m_mutexDeqConnections;
			std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

			asio::io_context m_asioContext;
			std::thread m_threadContext;
			
			//to get sockets of accepted clients
			asio::ip::tcp::acceptor m_asioAcceptor;

			//client id
			uint32_t nIDCounter = 1;//THIS MUST START AT 1. server uses this to partition ID's and reserves 0'th index as servers backup ids

			const uint16_t listeningPort;
	};
}