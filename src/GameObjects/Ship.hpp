#pragma once

#include "../NetGameObject.hpp"
#include "../IdGen.hpp"

class Ship : public NetGameObject<Ship> {
	public:
		NetGameObject::NetGameObject;

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