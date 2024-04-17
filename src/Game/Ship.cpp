#pragma once

#include "Ship.hpp"

void Ship::Update(float dt) {

}

void Ship::Draw(Canvas& c, Transformation_2D& transform) const {
	Vec2 former = Vec2(transform.offX(), transform.offY());
	for (Vec2 v : body) {
		transform.applyTo(v);
		c.DrawBlockLine(former.x, former.y, v.x, v.y);
		former = v;
	}
}

void Ship::packMessage(net::message<NetMsgType>& msg) {
	
}

void  Ship::unpackMessage(net::message<NetMsgType>& msg) {

}