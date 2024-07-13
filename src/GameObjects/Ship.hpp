#pragma once

#include "../NetGameObject.hpp"

#ifdef DrawText
#undef DrawText
#endif

class Ship : public NetGameObject<Ship> {
	public:
		NetGameObject::NetGameObject;

		void Draw(Canvas& c, Transformation_2D& transform) const override;
		void packMessage(Message&, MsgDiffType) override;
		void unpackMessage(Message&, MsgDiffType) override;

		const std::vector<Vec2_f> getBody() const;

	protected:
		std::vector<Vec2_f> body;
};