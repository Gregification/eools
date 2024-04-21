#pragma once

#include "GameObject.hpp"
#include "GameObjectFactory.hpp"
#include "GameStructs.hpp"	`q1

template<class T> //CRTP
class NetGameObject : public GameObject {
public:
	NetGameObject() : GameObject(BAD_ID) {}
	NetGameObject(inst_id nid) : GameObject(nid) {}

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

template<class T>
const GameObjectFactory NetGameObject<T>::gof = GameObjectFactory((static_cast<T*>(nullptr)));