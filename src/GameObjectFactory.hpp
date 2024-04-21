#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "GameObject.hpp"

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
	static class_id nextIdx;

	static std::shared_ptr<GameObject> getInstance(class_id id) {
		return std::move(ClassList[id]());
	}

	const class_id class_id;
};
