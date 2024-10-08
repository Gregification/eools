#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "NetMessageType.hpp"

//workaround for include loop
class GameObject;
typedef std::shared_ptr<GameObject> GameObjPtr;

class GameObjectFactory {
public:
	typedef std::function<GameObjPtr()> 
		InstanceConstructor;
	typedef std::function<GameObject* (GameObject*)>
		Kazathstan;//cast-ist-stan... ba dum tiss
	typedef std::function<void(Message&, MsgDiffType, GameObject*)>
		Pakistan;//packs and unpakcs stuff

	const Class_Id class_id;

	static Class_Id nextIdx;

	template<class T>
	GameObjectFactory(T* dummy) : class_id(nextIdx++)
	{
		static_assert(std::is_base_of<GameObject, T>::value, "attempted to register non gameobject class to the game object factory");
		
		//this dosent actually modify the ClassList vector before runtime(even though it does execute)
		//	seems to execute dependent on the compiler used. GOInit class solves this at the cost of not being dynamic :(
		//

		//packisar[class_id] = [](Message& msg, MsgDiffType diff, GameObject* go) { //packer
		//		((T*)go)->T::packMessage(msg, diff);
		//	};
	}

	static GameObjPtr GetInstance(Class_Id);

	static void UnpackAs(Class_Id, MsgDiffType, Message&, GameObject*);
	static void PackAs(Class_Id, MsgDiffType, Message&, GameObject*);

	//for testing. see commented out main in Tests/test.cpp
	static int getClassListSize() {
		return _instConstructors.size();
	}

	//named with underscore otherwise has naming conflict with windows api
	//this is only intended to run before anyhting else and only once.
	// see comments inside GameObjectFactory constructor for reasoning.
	//	tldr: compiler boogie? (it)
	static void Register_Class(
		Class_Id, 
		InstanceConstructor,
		Pakistan packer,
		Pakistan unpacker
	);

private:
	static std::vector<InstanceConstructor> _instConstructors;
	static std::vector<Pakistan> _packers,  _unpackers;
};
