#include "Loads.hpp"

const Unit& Loads::LoadHandler_IOPair::getInputR() const
{
	return i_r;
}

void Loads::LoadHandler_IOPair::setInputR(const float& r)
{
	i_r = r;
	recalOutput();
}

const Unit& Loads::LoadHandler_IOPair::getMaxDraw() const
{
	return max_draw;
}

void Loads::LoadHandler_IOPair::setMaxDraw(const Unit& m)
{
	max_draw = m;
	recalLoads();
}

void Loads::LoadHandler_IOPair::addLoad(Load l)
{
	if (l) {
		draw += l->draw;
		loads.push_back(l);
		recalLoads();
	}
}

std::vector<std::weak_ptr<Loads::LoadRequest>>& Loads::LoadHandler_IOPair::getLoads()
{
	return loads;
}

void Loads::LoadHandler_IOPair::removeLoad(Load l)
{
	if(l)
	loads.erase(std::remove_if(loads.begin(), loads.end(),
		[l](const std::weak_ptr<LoadRequest>& obj) {
			if(auto v = obj.lock())
				return v.get() == l.get(); 
			return true;
		}),
		loads.end());
}

void Loads::LoadHandler_IOPair::recalLoads()
{
	Unit actualLoad = 0;
	for (auto i = loads.begin(); i != loads.end();) {
		if (auto v = i->lock()) {
			actualLoad += v->draw;

			v->received = v->draw / draw * max_draw;

			++i;
		} else
			loads.erase(i);
	}

	//if load did change
	if (actualLoad != draw) {
		draw = actualLoad;
		for (auto i = loads.begin(); i != loads.end();) {
			if (auto v = i->lock()) {
				v->received = v->draw / draw * max_draw;
				++i;
			} else
				loads.erase(i);
		}
	}
}

void Loads::LoadHandler_IOPair::recalOutput()
{
	
}
