#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Game_common.hpp"
#include "Game/Grid.hpp"

class GameObject;

class GameObjectFactory {
	using InstanceConstructor = std::function<std::shared_ptr<GameObject>()>;

public:
	template<class T>
	GameObjectFactory(T* dummy) :
		class_id(nextIdx++)
	{
		static_assert(std::is_base_of<GameObject, T>::value, "attempted to register non gameobject class to the game object factory");

		InstanceConstructor initFunc = []() { return std::make_shared<T>(); };

		ClassList.push_back(
			initFunc
		);
	}

	static std::vector<InstanceConstructor> ClassList;
	static cid_t nextIdx;

	static std::shared_ptr<GameObject> getInstance(cid_t id);

	const cid_t class_id;
};
