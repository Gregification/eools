/**
* baselined off of javidx9 @ https://www.youtube.com/watch?v=2hNdkYInj4g
*/
#pragma once

#include "net_common.hpp"

namespace net {
	template <typename T>
	struct message_header {
		T id{};
		uint32_t size = 0;
	};

	template <typename T>
	struct message {
		message_header<T> header{};
		std::vector<uint8_t> body;

		size_t size() const {
			return sizeof(message_header<T>) + body.size();
		}

		void sizeBody(const size_t size) {
			body.resize(body.size() + size);
		}

		friend std::ostream& operator << (std::ostream& os, const message<T>& msg) {
			os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
			return os;
		}

		template<typename DT>
		friend message<T>& operator << (message<T>& msg, const DT& data) {
			static_assert(std::is_standard_layout<DT>::value, "too complex to complex to push");

			size_t i = msg.body.size();

			msg.body.resize(i + sizeof(DT));

			std::memcpy(msg.body.data() + i, &data, sizeof(DT));

			msg.header.size = msg.size();

			return msg;
		}

		template<typename DT>
		friend message<T>& operator >> (message<T>& msg, DT& data) {
			static_assert(std::is_standard_layout<DT>::value, "too complex to complex to push");

			size_t i = msg.body.size() - sizeof(DT);

			std::memcpy(&data, msg.body.data() + i, sizeof(DT));

			msg.body.resize(i);
			msg.header.size = msg.size();

			return msg;
		}

		//non distructive >> 
		template<typename DT>
		message<T>& peek(DT& data) const {
			static_assert(std::is_standard_layout<DT>::value, "nonguarenteed mem locations! :(");

			std::memcpy(
				&data,
				body.data() + body.size() - sizeof(DT),
				sizeof(DT)
			);

			return this;
		}
	};

	template <typename T>
	class connection;

	template <typename T>
	struct owned_message {
		std::shared_ptr<connection<T>> remote = nullptr;
		message<T> msg;

		friend std::ostream& operator << (std::ostream& os, const owned_message<T>& msg) {
			os << msg.msg;
			return os;
		}
	};
}