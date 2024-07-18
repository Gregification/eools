#pragma once

#include "GameObject.hpp"
#include "GameObjectFactory.hpp"

template<typename T>
class NetGameObject : public IdGen<T>, public GameObject {
public:
	using GameObject::GameObject;

	Class_Id GetClassId() const override {
		return IdGen<T>::gof.class_id;
	}

	static Class_Id ClassId() {
		return IdGen<T>::gof.class_id;
	}
};
