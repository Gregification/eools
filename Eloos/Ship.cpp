#pragma once

#include "Ship.hpp"

void Ship::Update(time_t dealtaTime, time_t currTime) {

}

void Ship::Draw(Canvas& c, const Vec2& offset, float scale) const {
	GameObject::Draw(c, offset, scale);
}

void Ship::packMessage(net::message<NetMsgType>& msg) const {

}

void  Ship::unpackMessage(net::message<NetMsgType>& msg) {

}