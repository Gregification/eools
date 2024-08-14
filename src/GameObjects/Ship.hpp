#pragma once

class Ship;

#include "../Game/Navigation.hpp"
#include "../NetGameObject.hpp"

#include "../Game/ship/Generator.hpp"
#include "../Game/ship/PowerBank.hpp"
#include "../Game/ship/DriveTrain.hpp"

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
		//not synched
		Navigation::NavInfo navinfo;
		DriveTrain driveTrain;

		//synched
		Power::PowerBank powerBank;
		Power::Generator powerSource;

		using NetGameObject::NetGameObject;

		const std::vector<Vec2_f> getBody() const;		
		void updateBody();

		void Update(float) override;
		void Draw(Canvas& c, Transformation_2D transform) override;
		void packMessage(Message&, MsgDiffType) override;
		void unpackMessage(Message&, MsgDiffType) override;

	protected:
		std::vector<Vec2_f> _body;
};
