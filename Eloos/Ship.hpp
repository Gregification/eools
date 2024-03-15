#pragma once

#include "GameObject.hpp"

class Ship : virtual GameObject {
public:
	void Draw(Canvas& c, Dimensions zoom) const override{
		GameObject::Draw(c, zoom);
	}

	void packMessage(net::message<NetMsgType>& msg) const override {
		GameObject::packMessage(msg);

	}

	void unpackMessage(net::message<NetMsgType>& msg) override {
		GameObject::unpackMessage(msg);
		
	}
};