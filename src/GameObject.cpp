#include "GameObject.hpp"

void GameObject::Draw(Canvas& c, Transformation_2D& trfmat) const {
	Vec2 pos = transform.position;
	trfmat.applyTo(pos);
	c.DrawBlock(pos.x, pos.y, true);
};

bool GameObject::NeedNetUpdate() {
	return needNetUpdate;
}

std::string GameObject::GetDescription() const {
	return "gameobject description";
}

std::string GameObject::getDisplayId() {
	return displayId;
}

std::string GameObject::getDisplayName() {
	return displayName;
}

void GameObject::setId(inst_id newId) {
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