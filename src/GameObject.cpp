#include "GameObject.hpp"

void GameObject::Draw(Canvas& c, const Vec2& offset, float scale) const {
	c.DrawBlock(transform.position.x, transform.position.y, true);
};

bool GameObject::NeedNetUpdate() {
	return needNetUpdate;
}

std::string GameObject::GetDescription() const {
	return "gameobject description";
}