#pragma once

#include "../GameObject.hpp"

class Ship : public GameObject {
	public:
		Ship(id_t id) : GameObject(id) {
			body = { Vec2(0,5), Vec2(-5,-5), Vec2(5,-5), Vec2(0,5)};
		}
		~Ship() {}
		
	public:
		std::vector<Vec2> body;

		float capacitor;

	public:
		void Update(float dt) override;

		void Draw(Canvas& c, const Vec2& offset, float scale) const override;

		void packMessage(net::message<NetMsgType>& msg) override;

		void unpackMessage(net::message<NetMsgType>& msg) override;
};