#pragma once

#include <set>

#include "../../NetMessageType.hpp"
#include "../CurvePipe.hpp"

namespace Loads{

	struct LoadRequest {
		LoadUnit requested = 0, received = 0;
		LoadUnit min = 0;
	};
	typedef std::shared_ptr<LoadRequest> Load;

	class LoadDistributor {
		LoadUnit total_requested;
		LoadUnit total_avaliable;
		LoadUnit total_used;

		//sort by ascending min
		struct cmper{
			bool operator()(const std::weak_ptr<LoadRequest>& a, const std::weak_ptr<LoadRequest>& b) const {
				auto as = a.lock();
				if (!as) return false;
				auto bs = b.lock();
				if (!bs) return true;
				return as->min < bs->min;
			}
		};
		std::multiset<std::weak_ptr<LoadRequest>, cmper> _loads;

	public:

		void addLoad(std::weak_ptr<LoadRequest>);
		void addLoadDontRecal(std::weak_ptr<LoadRequest>);
		void removeLoad(const LoadRequest*);

		void setTotalAvaliable(const LoadUnit&);

		const LoadUnit& totalRequested() const;
		const LoadUnit& totalAvaliable() const;
		const LoadUnit& totalUsed() const;

		void recalculate();
	};
};
