#pragma once

#include "net_common.hpp"

namespace net {
	/*thread safe queue, full-lock, */
	template<typename T>
	class tsqueue {
		public:
			tsqueue() = default;
			tsqueue(const tsqueue<T>&) = delete;

		public:
			const T& front() {
				std::scoped_lock lock(qMutex);
				return q.front();
			}

			const T& back() {
				std::scoped_lock lock(qMutex);
				return q.back();
			}

			T pop_front() {
				std::scoped_lock lock(qMutex);
				auto t = std::move(q.front());
				q.pop_front();
				return t;
			}
			
			T pop_back() {
				std::scoped_lock lock(qMutex);
				auto t = std::move(q.back());
				q.pop_back();
				return t;
			}

			const T& push_front(const T& item) {
				std::scoped_lock lock(qMutex);
				return q.emplace_front(std::move(item));
			}

			const T& push_back(const T& item) {
				std::scoped_lock lock(qMutex);
				return q.emplace_back(std::move(item));
			}

			bool isEmpty() {
				std::scoped_lock lock(qMutex);
				return q.empty();
			}

			size_t size() {
				std::scoped_lock lock(qMutex);
				return q.size();
			}

		protected:
			std::mutex qMutex;
			std::deque<T> q;

	};
}
