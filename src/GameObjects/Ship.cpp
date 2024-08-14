#pragma once

#include "Ship.hpp"

enum class DIFF : MsgDiffType {
	PowerBank = DEFAULT_MsgDiff_EVERYTHING + 1,
	PowerSource,
	Body
};

void Ship::Draw(Canvas& c, Transformation_2D t) {
	GameObject::Draw(c, t);

	auto pos = t.applyTo(transform.position);
	c.DrawText(pos.x - 4, pos.y, getPrettyString((float)id()) + "rot:" + getPrettyString(transform.rotation.getRotation()));
}

void Ship::Update(float dt){
	if (navinfo.navPattern)
		navinfo.navPattern->nav_update(dt, *this);
}

const std::vector<Vec2_f> Ship::getBody() const
{
	return _body;
}

void Ship::updateBody()
{
	addSynchronizationTarget(Ship::GetClassId(), DIFF::Body);
}

void Ship::packMessage(Message& msg, MsgDiffType t) {
	switch (t)
	{
	case static_cast<MsgDiffType>(DIFF::PowerSource):
		powerSource.packTargets(msg);		break;

	case static_cast<MsgDiffType>(DIFF::PowerBank):
		powerBank.packTargets(msg);			break;

	case static_cast<MsgDiffType>(DIFF::Body):
		packArray<Vec2_f>(msg, _body);	break;

	case DEFAULT_MsgDiff_EVERYTHING:
	default:
		packArray<Vec2_f>(msg, _body);
		powerSource.packTargets(msg);
		powerBank.packTargets(msg);
		break;
	}
}

void Ship::unpackMessage(Message& msg, MsgDiffType t) {
	switch (t)
	{
	case static_cast<MsgDiffType>(DIFF::PowerSource):
		powerSource.unpackTargets(msg);		break;

	case static_cast<MsgDiffType>(DIFF::PowerBank):
		powerBank.unpackTargets(msg);			break;

	case static_cast<MsgDiffType>(DIFF::Body):
		unpackArray<Vec2_f>(msg, _body);	break;

	case DEFAULT_MsgDiff_EVERYTHING:
	default:
		powerBank.unpackTargets(msg);
		powerSource.unpackTargets(msg);
		unpackArray<Vec2_f>(msg, _body);
		break;
	}
}
