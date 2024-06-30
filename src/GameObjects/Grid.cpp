#pragma once

#include "Grid.hpp"

//gold mine https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf
//https://www.youtube.com/watch?v=mD3cbXu3ZBE

void Grid::Update(float dt) {
	for (auto& go : gameObjects) {
		if (go.second)
			go.second->Update(dt);
	}
}

void Grid::Draw(Canvas& c, Transformation_2D& trf) const {
	for (auto& go : gameObjects) {
		if(go.second)
			go.second->Draw(c, trf);
	}
}

void Grid::packMessage(net::message<NetMsgType>& msg) {
	for (auto& v : gameObjects)
		msg << v.first;
	
	msg << gameObjects.size();
	msg << gridPos;
	msg << gridId;
}

void Grid::unpackMessage(net::message<NetMsgType>& msg) {
	msg >> gridId;
	msg >> gridPos;
	
	
}

void Grid::addGameObject(std::shared_ptr<GameObject>& go) {
	if (go->id() == BAD_ID)
		go->setId(LOCAL_PARITION.getNext());

	gameObjects.push_back({ go->id(), go});
}

std::optional<std::shared_ptr<GameObject>> Grid::getObject(Instance_Id id) {
	for (auto& v : gameObjects)
		if (v.first == id)
			return std::optional<std::shared_ptr<GameObject>>(v.second);

	return std::nullopt;
}

std::optional<std::shared_ptr<GameObject>> Grid::removeObject(Instance_Id id) {
	auto it = std::find_if(
		gameObjects.begin(),
		gameObjects.end(),
		[id](const std::pair<Instance_Id, std::shared_ptr<GameObject>>& e) 
			{ return e.first == id; }
	);

	if (it == gameObjects.end())
		return std::nullopt;

	gameObjects.erase(it);

	return std::optional<std::shared_ptr<GameObject>>(it->second);
}
