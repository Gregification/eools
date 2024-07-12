#include "GameObjectFactory.hpp"
#include "GameObject.hpp"
#include "IdGen.hpp"

Class_Id GameObjectFactory::nextIdx = 0;

std::vector<GameObjectFactory::InstanceConstructor> GameObjectFactory::_instConstructors{};
std::vector<GameObjectFactory::Pakistan> 
	GameObjectFactory::_packers{},
	GameObjectFactory::_unpackers{};

GameObjPtr GameObjectFactory::GetInstance(Class_Id cid) {
	return _instConstructors[cid]();
}

void GameObjectFactory::UnpackAs(Class_Id cid, MsgDiffType diff, Message& msg, GameObject* go) {
	_unpackers[cid](msg, diff, go);
}

void GameObjectFactory::PackAs(Class_Id cid, MsgDiffType diff, Message& msg, GameObject* go) {
	_packers[cid](msg, diff, go);
}

void GameObjectFactory::Register_Class(
		Class_Id cid,
		InstanceConstructor ic,
		Pakistan packer,
		Pakistan unpacker
	) {
	assert(cid < nextIdx);

	if (_instConstructors.size() < nextIdx) {
		_instConstructors.resize(nextIdx);
		_packers.resize(nextIdx);
		_unpackers.resize(nextIdx);
	}

	_instConstructors[cid] = ic;
	_packers[cid] = packer;
	_unpackers[cid] = unpacker;
}
