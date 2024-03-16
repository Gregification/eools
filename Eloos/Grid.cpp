#pragma once

#include "Grid.hpp"

void Grid::Update(time_t dt, time_t ct) {
	for (auto& go : GameObjects) {
		if (go.second)
			go.second->Update(dt, ct);
	}
}

void Grid::Draw(Canvas& c, const Vec2& offset, float scale) const {
	GameObject::Draw(c, offset, scale);
}

void Grid::packMessage(net::message<NetMsgType>& msg) const {
	GameObject::packMessage(msg);

	packArray(
		msg,
		&GameObjects[0],
		GameObjects.size(),
		GId_pair::util_packArray
	);
	
	msg << gridPos;
	msg << gridId;
}

void  Grid::unpackMessage(net::message<NetMsgType>& msg) {
	id_t msg_id;
	msg >> msg_id;
	msg >> gridPos;

	if (gridId == NAN || msg_id != gridId) {
		gridId = msg_id;
		GameObjects.clear();
	} else
		msg_id = 0;

	unpackArray(
		msg,
		GameObjects,
		GId_pair::util_unpackArray
	);

	//if not baselining off server
	if(msg_id) {//filter, make unique
		auto last = std::unique(GameObjects.begin(), GameObjects.end());
		GameObjects.resize(std::distance(GameObjects.begin(), last));
	}

	needNetUpdate = false;

	GameObject::unpackMessage(msg);
}

void Grid::addGameObject(GameObject& go) {
	GId_pair gp;
}

bool Grid::NeedNetUpdate() {
	return GameObject::needNetUpdate;
}