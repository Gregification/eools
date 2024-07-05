#pragma once

#include <memory>

class GameObject;
class GameObjectFactory;

template<class T> //CRTP
class IdGen {
	friend class GOInit;
public:
	const static GameObjectFactory gof;

private:
	void static init() {
		static bool once = false;
		if (once) return;
		once = true;

		GameObjectFactory::Register_Class(
			gof.class_id,
			[] { return std::make_shared<T>(); },
			[](const std::shared_ptr<GameObject>& ptr) { return std::dynamic_pointer_cast<T>(ptr); }
		);
	};
};

template<class T>
const GameObjectFactory IdGen<T>::gof = GameObjectFactory(static_cast<T*>(nullptr));