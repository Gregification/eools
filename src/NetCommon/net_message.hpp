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
		std::vector<uint8_t> _body;

		size_t size() const {
			return _body.size();
		}

		void sizeBody(const size_t size) {
			_body.resize(_body.size() + size);
		}

		friend std::ostream& operator << (std::ostream& os, const message<T>& msg) {
			os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
			return os;
		}

		template<typename DT>
		friend message<T>& operator << (message<T>& msg, const DT& data) {
			static_assert(std::is_standard_layout<DT>::value, "too complex to complex to push");

			size_t i = msg._body.size();

			msg._body.resize(i + sizeof(DT));

			std::memcpy(msg._body.data() + i, &data, sizeof(DT));

			msg.header.size = msg.size();

			return msg;
		}

		template<typename DT>
		friend message<T>& operator >> (message<T>& msg, DT& data) {
			static_assert(std::is_standard_layout<DT>::value, "too complex to complex to push");

			size_t i = msg._body.size();
			if (i >= sizeof(DT)) {
				i -= sizeof(DT);

				std::memcpy(&data, msg._body.data() + i, sizeof(DT));

				msg._body.resize(i);
			}
			msg.header.size = msg.size();
			return msg;
		}

		//non distructive >> 
		template<typename DT>
		message<T>& peek(DT& data) const {
			static_assert(std::is_standard_layout<DT>::value, "nonguarenteed mem locations! :(");

			std::memcpy(
				&data,
				_body.data() + _body.size() - sizeof(DT),
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