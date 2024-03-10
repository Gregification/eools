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
		public:
			client_interface() : m_socket(m_context) {

			}
			
			~client_interface() {
				Disconnect();
			}

		public:
			bool Connect(const std::string& host, const uint16_t port) {
				try { //establish asio connection
					m_connection = std::make_unique<connection<T>>(); //TODO

					//resolve host string to something asio can target
					asio::ip::tcp::resolver resolver(m_context);
					asio::ip::tcp::resolver::results_type m_endpoints = resolver.resolve(host, std::to_string(port)); //throws exception on bad input
					
					m_connection->ConnectToServer(m_endpoints);

					thrContext = std::thread([this]() {m_context.run(); });
				} catch (std::exception& e) { //intended for asio exceptions
					std::cerr << "panic~ mayhem~ a cl9ent connection exception!: " << e.what() << "\n";
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

			void isConnected() {
				if(m_connection)
					return m_connection->isConnected();
				return false;
			}

			tsqueue<owned_message<T>>& Incomming() {
				return m_qMessagesIn;
			}

		protected:
			/*asio context used*/
			asio::io_context m_context;
			/*thread running the asio context*/
			std::thread thrContext;
			/*socket to server*/
			asio::ip::tcp::socket m_socket;
			/*connected instance of socket, handles data transfer*/
			std::unique_ptr<connection<T>> m_connection;

		private:
			/*messages from server*/
			tsqueue<owned_message<T>> m_qMessagesIn;
	};
}