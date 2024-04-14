#pragma once

#include "../GameObject.hpp"

class Ship : public GameObject {
	public:
		const static GameObjectFactory gof;

		Ship() : Ship(BAD_ID) {}
		Ship(id_t id) : GameObject(id) {
			body = { Vec2(0,5), Vec2(-5,-5), Vec2(5,-5), Vec2(0,5)};
		}
		~Ship() {}

	public:
		std::vector<Vec2> body;

		float capacitor = 0;

	public:
		void Update(float dt) override;

		void Draw(Canvas& c, Transformation_2D& transform) const override;

		void packMessage(net::message<NetMsgType>& msg) override;

		void unpackMessage(net::message<NetMsgType>& msg) override;
};