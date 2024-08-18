#pragma once

#include "../../Game_common.hpp"

class Ship;

struct DriveTrain {
	//this entire thing is a mess, not sure how to approach this.
	//	just enough that it does something for now.

	//u : universal / non-directional
	//d : direcitonal (TODO: ill figure out the details later)

	float u_rotAccele = M_PI / 10;
	Unit u_accele = 0.001 * DIST_KM;

	float accele_throttle = 0.0;

	void update(const float& dt, Ship&);
};
