#pragma once

class GameObjectFactory;

template<class T> //CRTP
class IdGen {
public:
	const static GameObjectFactory gof;
};

template<class T>
const GameObjectFactory IdGen<T>::gof = GameObjectFactory((static_cast<T*>(nullptr)));
