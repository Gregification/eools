#pragma once


#include "Ship.hpp"

void Ship::Draw(Canvas& c, Transformation_2D t) const {
	GameObject::Draw(c, t);

	t.mat = t.mul(transform.getRotationTransformation());
	auto pos = t.applyTo(transform.position);
	c.DrawText(pos.x - 4, pos.y, getPrettyString((float)id()));
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
	
}

const std::vector<Vec2_f> Ship::getBody() const
{
	return _body;
}
