#pragma once

#include "../GameObject.hpp"

class Ship : virtual GameObject {
	public:
		Ship(id_t id) : GameObject(id) {

		}
		~Ship() {}

		float capacitor;

		std::vector<Vec2> body;

	public:
		void Update(time_t dealtaTime, time_t currTime) override;

		void Draw(Canvas& c, const Vec2& offset, float scale) const override;

		void packMessage(net::message<NetMsgType>& msg) const override;

		void unpackMessage(net::message<NetMsgType>& msg) override;
};