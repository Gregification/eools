#pragma once

class Ship;

#include "../Game/Navigation.hpp"
#include "../NetGameObject.hpp"

#ifdef DrawText
#undef DrawText
#endif

namespace Navigation {
	struct NavBase;

	struct NavInfo : Messageable {
		std::unique_ptr<NavBase> navPattern;

		void setNavPattern(std::unique_ptr<NavBase>);

		void packMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
		void unpackMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
	};
}

class Ship : public NetGameObject<Ship> {
	public:
		using NetGameObject::NetGameObject;

		void Draw(Canvas& c, Transformation_2D transform) override;
		void packMessage(Message&, MsgDiffType) override;
		void unpackMessage(Message&, MsgDiffType) override;

		void Update(float);

		const std::vector<Vec2_f> getBody() const;		
		
		Navigation::NavInfo navinfo;

		float rotAccele = M_PI_4;
		float accele = 1;

	protected:
		std::vector<Vec2_f> _body;
};
