#pragma once


#include "Ship.hpp"

void Ship::Draw(Canvas& c, Transformation_2D& trans) const {
	#define tra transform
	#define pos tra.position

	float scale = 3;
	c.DrawBlockCircle(pos.x, pos.y, scale);
	
	Transformation_2D mod = tra.getRotationTransformation();
	Vec2 
		start(0, scale),
		end(0, scale * 3);
	/*start = mod.applyTo(start);
	end = mod.applyTo(end);*/

	c.DrawBlockLine(pos.x + start.x, pos.y + start.y, pos.x + end.x, pos.y + end.y, Color::Green);
}

void Ship::packMessage(Message& msg, MsgDiffType) {
	
}

void Ship::unpackMessage(Message& msg, MsgDiffType) {
	
}