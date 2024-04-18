#pragma once

#include "GameObject.hpp"
#include "GameObjectFactory.hpp"
#include "GameStructs.hpp"	`q1

template<class T> //CRTP
class NetGameObject : public GameObject {
public:
	NetGameObject() : GameObject(BAD_ID) {}
	NetGameObject(id_t nid) : GameObject(nid) {}

	const static GameObjectFactory gof;

	GameObjectFactory getGOF() const override;

	//yeah
	static GameObjectFactory staticGetGOF();

	ID getID() const override;
};
