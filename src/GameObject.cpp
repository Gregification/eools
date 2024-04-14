#include "GameObject.hpp"

std::vector<std::function<std::shared_ptr<GameObject>()>> GameObjectFactory::ClassList = std::vector<std::function<std::shared_ptr<GameObject>()>>();
size_t GameObjectFactory::nextIdx = 0;

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