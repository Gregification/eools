#pragma once

#include "../NetGameObject.hpp"

class Ship : public NetGameObject<Ship> {
	public:
		Ship(id_t nid) : NetGameObject(nid) {
		}
		Ship() : NetGameObject() {
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