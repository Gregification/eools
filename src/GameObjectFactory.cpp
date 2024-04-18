#include "GameObjectFactory.hpp"

std::vector<GameObjectFactory::InstanceConstructor> GameObjectFactory::ClassList = std::vector<GameObjectFactory::InstanceConstructor>(4);
cid_t GameObjectFactory::nextIdx = 0;

std::shared_ptr<GameObject> GameObjectFactory::getInstance(cid_t id) {
	return std::move(ClassList[id]());
}