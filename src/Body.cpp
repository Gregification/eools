#include "Body.hpp"

using namespace gs;

void Body::recalAABB() {

	coverage.pos = coverage.size = verticies[0];

	for (int i = 1; i < verticies.size(); i++) {
		//std::max\min isnt happy with templates

		//coverage.pos  (NW): smallest xy
		//coverage.size (SE): largest xy

		if (coverage.pos.x > verticies[i].x)
			coverage.pos.x = verticies[i].x;
		else if (coverage.size.x < verticies[i].x)
			coverage.size.x = verticies[i].x;

		if (coverage.pos.y > verticies[i].y)
			coverage.pos.y = verticies[i].y;
		else if (coverage.size.y < verticies[i].y)
			coverage.size.y = verticies[i].y;
	}

	//global max to size
	coverage.size -= coverage.pos;

	auto rawsize = coverage.size;

	//adjust so that it coveragean be rotated however
	// pythagorean
	coverage.size *= coverage.size;
	coverage.size *= 2;
	coverage.size.x = std::sqrtf(coverage.size.x);
	coverage.size.y = std::sqrtf(coverage.size.y);

	//adjust to center
	coverage.pos -= (coverage.size - rawsize) / 2;
}

gs::Rectangle Body::getAABB() {
	if (coverage.getArea() == 0 && verticies.size() > 1)
		recalAABB();

	return coverage;
}

/*
* code taken from 
* https://wrfranklin.org/Research/Short_Notes/pnpoly.html
*/
bool Body::isPointInBody(Vec2_f p) const
{
	bool out = false;

	int a = verticies.size()-1, b = 0;

	//better explained from stack overflow
	for (int b = 0; b < verticies.size(); a = b++) {
		if ((verticies[b].y > p.y) != (verticies[a].y > p.y) &&
			(p.x < (verticies[a].x - verticies[b].x) * (p.y - verticies[b].y) / (verticies[a].y - verticies[b].y) + verticies[b].x)
			)
			out = !out;
	}

	return out;
}
