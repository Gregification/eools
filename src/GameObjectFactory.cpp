#pragma once

#include "GameObjectFactory.hpp"
#include "GameObject.hpp"
#include "GameStructs.hpp"

std::vector<GameObjectFactory::InstanceConstructor> GameObjectFactory::ClassList{};
std::vector<GameObjectFactory::CastFunction> GameObjectFactory::CastList{};
Class_Id GameObjectFactory::nextIdx = 0;

std::shared_ptr<GameObject> GameObjectFactory::GetInstance(Class_Id id) {
	return ClassList[id]();
}

std::shared_ptr<GameObject> GameObjectFactory::CastTo(std::shared_ptr<GameObject>& ptr, Class_Id cid) {
	assert(cid < CastList.size());
	return CastList[cid](ptr);
}

void GameObjectFactory::Register_Class(Class_Id cid, InstanceConstructor ic, CastFunction cf) {
	assert(cid < nextIdx);
	if (ClassList.size() < nextIdx) {
		ClassList.resize(nextIdx);
		CastList.resize(nextIdx);
	}

	ClassList[cid] = ic;
	CastList[cid] = cf;
}
