#pragma once

#include "Grid.hpp"
#include "../GameObject.hpp"

//gold mine https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf
//https://www.youtube.com/watch?v=mD3cbXu3ZBE

void Grid::Update(float dt) {
	for (auto& [id, go] : gameObjects) {
		if (go)
			go->Update(dt);
	}
}

void Grid::Draw(Canvas& c, Transformation_2D& trf) const {
	for (auto& [id, go] : gameObjects) {
		if(go)
			go->Draw(c, trf);
	}
}

void Grid::packMessage(net::message<NetMsgType>& msg) {
	GameObject::packMessage(msg);

	for (auto& [id, go] : gameObjects) msg << id;

	msg << gameObjects.size();
	msg << gridPos;
	msg << gridId;
}

void Grid::unpackMessage(net::message<NetMsgType>& msg) {
	inst_id msg_id;
	msg >> msg_id;
	msg >> gridPos;

	if (gridId == BAD_ID || msg_id != gridId) {
		gridId = msg_id;
		gameObjects.clear();
	} else
		msg_id = 0;

	size_t l;
	msg >> l;
	for (int i = 0; i < l; i++) {
		inst_id id;
		msg >> id;

		if (!msg_id) { //if NOT baselining from server
			if (gameObjects.find(id) != gameObjects.end()) //if already exists
				continue;
		}
		gameObjects.insert({id, std::shared_ptr<GameObject>(nullptr)});
	}

	needNetUpdate = false;

	GameObject::unpackMessage(msg);
}


bool Grid::NeedNetUpdate() {
	return GameObject::needNetUpdate;
}

void Grid::addGameObject(std::shared_ptr<GameObject>& go) {
	if (go->id() == BAD_ID)
		go->setId(LOCAL_PARITION.getNext());

	gameObjects.insert({ go->id(), go});
}

std::shared_ptr<GameObject> Grid::getObject(inst_id id) {
	auto ret = gameObjects.find(id);

	if (ret == gameObjects.end())
		return std::shared_ptr<GameObject>(nullptr);
	else
		return ret->second;
}

std::shared_ptr<GameObject> Grid::removeObject(inst_id id) {
	auto it = gameObjects.find(id);
	if(it == gameObjects.end())
		return std::shared_ptr<GameObject>(nullptr);

	gameObjects.erase(it);

	return it->second;
}
