#pragma once

#include "Ship.hpp"

void Ship::Draw(Canvas& c, Transformation_2D t) {
	GameObject::Draw(c, t);

	auto pos = t.applyTo(transform.position);
	c.DrawText(pos.x - 4, pos.y, getPrettyString((float)id()) + "rot:" + getPrettyString(transform.rotation.getRotation()));
}

void Ship::packMessage(Message& msg, MsgDiffType diff) {
	switch (diff) {
		default:
		case DEFAULT_MsgDiff_EVERYTHING: {
			packArray<Vec2_f, Vec2_f>(msg, _body);
		} break;
	}
}

void Ship::unpackMessage(Message& msg, MsgDiffType diff) {
	switch (diff) {
		default:
		case DEFAULT_MsgDiff_EVERYTHING: {
			unpackArray<Vec2_f>(msg, _body);
		} break;
	}
}

void Ship::Update(float dt){
	if (navinfo.navPattern)
		navinfo.navPattern->nav_update(dt, *this);
}

const std::vector<Vec2_f> Ship::getBody() const
{
	return _body;
}
