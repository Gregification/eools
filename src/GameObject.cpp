#include "GameObject.hpp"

std::vector<GameObjectFactory::InstanceConstructor> GameObjectFactory::ClassList = std::vector<GameObjectFactory::InstanceConstructor>(4);
cid_t GameObjectFactory::nextIdx = 0;

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

ID GameObject::getID() const {
	ID ret = ID();

	return ret;
}