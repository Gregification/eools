#pragma once

#include "NetGameObject.hpp"

//DO NOT USE THIS FILE
//some thing during the build (cmake i suspect) recoginizes this file 
//	however it fails during linking.

//template<class T>
//const GameObjectFactory NetGameObject<T>::gof = GameObjectFactory((static_cast<T*>(nullptr)));

//template<class T>
//GameObjectFactory NetGameObject<T>::getGOF() const {
//	return staticGetGOF;
//}
//
//template<class T>
//GameObjectFactory NetGameObject<T>::staticGetGOF() {
//	return gof;
//}

//template<class T>
//ID NetGameObject<T>::getID() const {
//	auto idee = ID();
//	idee.classId = gof.class_id;
//	idee.instanceId = id;
//
//	return idee;
//}
