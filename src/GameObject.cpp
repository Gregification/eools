#include "GameObject.hpp"
#include "IdGen.hpp"
#include "GameObjectFactory.hpp"

const Vec2 Vec2::BAD = Vec2(NAN, NAN);

void GameObject::FixedUpdate(float dt) {
	transform.FixedUpdate(dt);
}

void GameObject::Draw(Canvas& c, Transformation_2D& trfmat) const {
	Vec2 pos = transform.position;
	trfmat.applyTo(pos);
	c.DrawBlock(pos.x, pos.y, true);
};

Class_Id GameObject::GetClassId() const {
	return IdGen<GameObject>::gof.class_id;
}

void GameObject::packMessage(Message& msg) {
	msg << transform;
	msg << mass;
}

void GameObject::unpackMessage(Message& msg) {
	msg >> mass;
	msg >> transform;
}

bool GameObject::NeedsNetUpdate() {
	return true;
}

std::string GameObject::GetDescription() const {
	return "default gameobject description";
}

std::string GameObject::getDisplayId() {
	return displayId;
}

std::string GameObject::getDisplayName() {
	return displayName;
}

void GameObject::setId(Instance_Id newId) {
	instId = newId;

	if (instId == BAD_ID) {
		displayId = "BAD ID";
	}
	else {
		std::stringstream ss;
		ss << std::hex << std::uppercase << instId;
		displayId = ss.str();
	}
}

void GameObject::setDisplayName(std::string dispName) {
	displayName = dispName;
}