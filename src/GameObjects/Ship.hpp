#pragma once

#include "../NetGameObject.hpp"
#include "../Game/Navigation.hpp"

#ifdef DrawText
#undef DrawText
#endif

class Ship : public NetGameObject<Ship> {
	public:
		using NetGameObject::NetGameObject;

		void Draw(Canvas& c, Transformation_2D& transform) const override;
		void packMessage(Message&, MsgDiffType) override;
		void unpackMessage(Message&, MsgDiffType) override;

		void Update(float);

		const std::vector<Vec2_f> getBody() const;		
		
		Navigation::Navigator navigator;

	protected:
		std::vector<Vec2_f> _body;
};
