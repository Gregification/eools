#pragma once

#include "../NetGameObject.hpp"

#ifdef DrawText
#undef DrawText
#endif

class Ship : public NetGameObject<Ship> {
	public:
		NetGameObject::NetGameObject;

		~Ship() {}

	public:
		std::vector<Vec2> body;

		float capacitor = 0;

	public:
		void Draw(Canvas& c, Transformation_2D& transform) const override;

		void packMessage(Message&, MsgDiffType = 0) override;
		void unpackMessage(Message&, MsgDiffType = 0) override;
};