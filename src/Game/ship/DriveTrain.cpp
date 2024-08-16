#include "DriveTrain.hpp"
#include "../../GameObjects/Ship.hpp"

//https://www.google.com/search?client=firefox-b-1-d&q=keplerian+orbit+programtic+calculator

void DriveTrain::update(const float& dt, Ship& s)
{
	s.transform.applyInlineVelocity(
		static_cast<double>(dt) * s.driveTrain.accele_throttle * s.driveTrain.u_accele
	);
}
