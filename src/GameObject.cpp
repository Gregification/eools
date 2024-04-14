#include "GameObject.hpp"

std::unordered_map<cid_t, std::function<std::shared_ptr<GameObject>()>> GameObjectFactory::ClassList = std::unordered_map<cid_t, std::function<std::shared_ptr<GameObject>()>>();
cid_t GameObjectFactory::nextIdx = BAD_ID+1;

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