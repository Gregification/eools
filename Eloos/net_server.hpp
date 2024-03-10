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
				: m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), listeningPort)) 
			{

			}

			virtual ~server_interface() {
				Stop();
			}

			bool Start() {
				try {
					WaitForClientConnection();//init first task. context will otherwise immideatly exit

					m_threadContext = std::thread([this]() {m_asioContext.run()});
				} catch (std::exception& e) {
					onError("[SERVER] Exception: " << e.what());
					return false;
				}

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
						if (ec)
							onError("[SERVER] New Connection Error: " << ec.message());
						else {
							std::shared_ptr<connection<T>> newconn =
								std::make_shared<connection<T>>(connection<T>::owner::server,
									m_asioContext, std::move(socket), m_qMessagedIn);

							//filter connection
							if (OnClientConnect(newconn)) {
								m_deqConnections.push_back(std::move_backward(newconn));

								//assign unique id (yeah it'll eventually overflow but eh, congrats if your server gets that popular)
								m_deqConnections.back()->ConnectToClient(nIDCounter++);

								onEvent("connection accepted [id:" << n_deqConnections.back()->GetId() <<"]");
							} else {
								onEvent("connection denied");
							}
						}//16:59
						//reprime asio context 
						WaitForClientConnection();
					});
			}

			//send message to specific client
			void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg) {

			}

			//broadcast
			void MessageAllClients(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr) {

			}

		protected:
			//returns true/accept or false/decline regarding the connection. think Swing predicate filters
			virtual bool OnClientConnect(std::shared_ptr<connection<T>> client) {
				return false;
			}

			virtual void onClientDisconnect(std::shared_ptr<connection<T>> client) {

			}

			//on message from specific given client
			virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg) {

			}

			//for user purposes, good luck tring to use this for anyhting otherwise. rip
			virtual void onError(std::string message) {
				cerr << message;
			}
			
			//on notable evets such as connection being denied, user joining and so on
			virtual void onEvent(std::string message) {
				cerr << message << "\a\n";
			}

		protected:
			//incomming
			tsqueue<owned_message<T>> m_qMessagedIn;

			//active validated connections
			std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

			asio::io_context m_asioContext;
			std::thread m_threadContext;
			
			//to get sockets of accepted clients
			asio::ip::tcp::acceptor m_asioAcceptor;

			//client id
			uint32_t nIDCounter = 10000;
	};
}