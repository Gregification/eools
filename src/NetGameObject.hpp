#pragma once

#include "GameObject.hpp"

template<typename T>
class NetGameObject : public IdGen<T>, public GameObject {
public:
	GameObject::GameObject;

	Class_Id GetClassId() const override {
		return IdGen<T>::gof.class_id;
	}
	
};
