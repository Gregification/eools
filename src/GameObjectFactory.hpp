#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Game_common.hpp"
#include "GameObject.hpp"

class GameObject;

class GameObjectFactory {
	using InstanceConstructor = std::function<std::shared_ptr<GameObject>()>;

public:
	template<class T>
	GameObjectFactory(T* dummy) 
		: class_id(nextIdx++)
	{
		static_assert(std::is_base_of<GameObject, T>::value, "attempted to register non gameobject class to the game object factory");

		ClassList.push_back([]() {
			return std::make_shared<T>(); 
		});

	}

	static Class_Id nextIdx;

	static std::shared_ptr<GameObject> getInstance(Class_Id id) {
		//ClassList is alwayse empty when this is called
		//BUT the constructor for this class is guarenteed to have been called
		//which guarentees ClassList has at least something in it... but it dosent 
		return ClassList[id]();
	}

	const Class_Id class_id;

private: //private to guarentee only this class can access this vector
	static std::vector<InstanceConstructor> ClassList;
};
