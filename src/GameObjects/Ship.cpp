#pragma once


#include "Ship.hpp"

void Ship::Draw(Canvas& c, Transformation_2D t) const {
	GameObject::Draw(c, t);

	t.mat = t.mul(transform.getRotationTransformation());
	auto pos = t.applyTo(transform.position);
	c.DrawText(pos.x - 4, pos.y, getPrettyString((float)id() / IDPartition::LOCAL_PARITION.min));
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

void Ship::Update(float){
	switch (navinfo.travelState) {
		case Navigation::TRAVEL_STATE::NONE: {

			}break;

		case Navigation::TRAVEL_STATE::ALIGN: {
				navinfo.destInfo.align.targetRot;
			}break;

		case Navigation::TRAVEL_STATE::ALIGN_TO: {

			}break;

		case Navigation::TRAVEL_STATE::APPROACH: {

			}break;

		case Navigation::TRAVEL_STATE::MAINTAIN_DISTANCE: {

			}break;

		case Navigation::TRAVEL_STATE::ORBIT: {

			}break;
	}
}

const std::vector<Vec2_f> Ship::getBody() const
{
	return _body;
}
