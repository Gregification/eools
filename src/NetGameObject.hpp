#pragma once

#include "GameObject.hpp"
#include "GameObjectFactory.hpp"

template<class T> //CRTP
class NetGameObject : public GameObject {
public:
	NetGameObject() : GameObject(BAD_ID) 
		{}

	const static GameObjectFactory gof;

	GameObjectFactory getGOF() const override {
		return gof;
	}

	//yeah
	static GameObjectFactory staticGetGOF() {
		return gof;
	}

	ID getID() const override {
		auto idee = ID();
			idee.classId = gof.class_id;
			idee.instanceId = id;

		return idee;
	}
};
