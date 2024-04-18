#pragma once

#include "NetGameObject.hpp"

template<class T>
const GameObjectFactory NetGameObject<T>::gof = GameObjectFactory((static_cast<T*>(nullptr)));

template<class T>
GameObjectFactory NetGameObject<T>::getGOF() const {
	return staticGetGOF;
}

template<class T>
GameObjectFactory NetGameObject<T>::staticGetGOF() {
	return gof;
}

template<class T>
ID NetGameObject<T>::getID() const {
	auto idee = ID();
	idee.classId = gof.class_id;
	idee.instanceId = id;

	return idee;
}
