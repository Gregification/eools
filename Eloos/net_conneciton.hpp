#pragma once

#include "net_common.hpp"
#include "net_tsqueue.hpp"
#include "net_message.hpp"

namespace net {
	template <typename T>
	class connection : public std::enable_shared_from_this<connection<T>> {
		public:
			connection() {}

			virtual ~connection() {};

		public:
			bool connectToServer();
			bool disconnect();
			bool isConnected() const;
			bool Send(const message<T>& msg);

		//asio
		protected:
			asio::ip::tcp::socket m_socket; //unique remote socket
			asio::io_context& m_asioContext;

			//outgoing messages
			tsqueue<message<T>> m_qMessagesOut;

			//messages received from remote
			tsqueue<owned_message>& m_qMessagesIn;

	};
}