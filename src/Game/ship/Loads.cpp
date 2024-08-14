#include "Loads.hpp"

using namespace Loads;

void Loads::LoadDistributor::addLoad(std::weak_ptr<LoadRequest> p)
{
	if (auto s = p.lock()) {
		_loads.insert({ p });
		recalculate();
	}
}

void Loads::LoadDistributor::addLoadDontRecal(std::weak_ptr<LoadRequest> p)
{
	_loads.insert({ p });
}

void Loads::LoadDistributor::removeLoad(const LoadRequest* p)
{
	bool needsRecal = false;

	for (auto i = _loads.begin(); i != _loads.end();) {
		Load lr = i->lock();

		if (!lr || lr.get() == p) {
			needsRecal = true;

			_loads.erase(i);
		}
		else 
			++i;
	}

	if(needsRecal)
		recalculate();
}

void Loads::LoadDistributor::setTotalAvaliable(const LoadUnit& n)
{
	total_avaliable = n;
	total_used = 0;

	LoadUnit totalExtraOfSupported = 0;
	LoadUnit remaining = total_avaliable;

	//hit as many minimums as possible
	auto lastSupported = _loads.begin();
	for (; lastSupported != _loads.end(); ++lastSupported)
		if (Load lr = lastSupported->lock()) {
			if (lr->min < remaining) {

				lr->received = lr->min;
				
				remaining -= lr->min;
				totalExtraOfSupported += lr->requested - lr->min;
			} 
			else
				break;
		}
		else {
			recalculate();
		}

	//for all reached mins, distribute relative to extra
	for (auto i = _loads.begin(); i != lastSupported; ++i)
		if (Load lr = i->lock()) {
			LoadUnit allotted = (lr->requested - lr->min) / totalExtraOfSupported * remaining;

			lr->received += allotted;
			total_used += lr->received;
		}
}

void Loads::LoadDistributor::recalculate()
{
	total_requested = 0;

	for (auto i = _loads.begin(); i != _loads.end(); ++i)
		if (Load lr = i->lock()) {
			total_requested += lr->requested;
		}
		else {
			_loads.erase(i, _loads.end());
			break;
		}

	setTotalAvaliable(total_avaliable);
}

const LoadUnit& Loads::LoadDistributor::totalRequested() const
{
	return total_requested;
}

const LoadUnit& Loads::LoadDistributor::totalAvaliable() const
{
	return total_avaliable;
}

const LoadUnit& Loads::LoadDistributor::totalUsed() const
{
	return total_used;
}
