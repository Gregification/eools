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
	/*im losing it
	* GameObjectFactory has a prupose that is to be able to
	* instiantiate any GameObject form a given ClassId, it does so by registering a
	* funciton to a static list for every class that extends it. this class is what
	* lets it be done for every class dynamically, it passes a template to the
	* GOF constructor, BUT these static constructors are triggered some point
	* between compile and run time (vss call stack says during "EOOLS.exe!
	* `dynamic initializer for 'IdGen<Grid>::gof''", idk if thats the offical term)
	* and are NOT ENTIRELY PRESISTANT to runtime. end result is that changes to
	* primitives (and maybe some other object types) presist while changes to
	* something like a vector does not. I have no idea how to fix this without
	* instiantiating every class manually in a massive list, which defeats the
	* point of all this...
	*/

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