#pragma once

#include "Ship.hpp"

void Ship::Update(time_t dealtaTime, time_t currTime) {

}

void Ship::Draw(Canvas& c, const Vec2& offset, float scale) const {
	Vec2 bodyStart = offset;
	for (Vec2 v : body) {
		v += offset;
		c.DrawBlockLine(bodyStart.x, bodyStart.y, v.x, v.y);
		bodyStart = v;
	}
}

void Ship::packMessage(net::message<NetMsgType>& msg) const {

}

void  Ship::unpackMessage(net::message<NetMsgType>& msg) {

}