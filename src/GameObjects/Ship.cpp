#pragma once


#include "Ship.hpp"

void Ship::Draw(Canvas& c, Transformation_2D& trans) const {
	c.DrawText(transform.position.x, transform.position.y,"s" + std::to_string(id()));
	/*Vec2 former = Vec2(transform.offX(), transform.offY());
	for (Vec2 v : body) {
		transform.applyTo(v);
		c.DrawBlockLine(former.x, former.y, v.x, v.y);
		former = v;
	}*/
}

void Ship::packMessage(net::message<NetMsgType>& msg) {
	//GameObject::packMessage(msg);
	int a = 0;
}

void  Ship::unpackMessage(net::message<NetMsgType>& msg) {
	//GameObject::unpackMessage(msg);
	int a = 0;
}