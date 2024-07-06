#pragma once

#include <span>
#include "Grid.hpp"

#ifdef DrawText
#undef DrawText
#endif

//gold mine https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf
//https://www.youtube.com/watch?v=mD3cbXu3ZBE

void Grid::Update(float dt) {
	
}

void Grid::Draw(Canvas& c, Transformation_2D& trf) const {
	c.DrawText(20, 20, "drawing grid w/ id: " + std::to_string(id()));
}

void Grid::packMessage(Message& msg) {
	packArray<GOObj, Instance_Id> (
		msg,
		_go_vec,
		[](GOObj& obj) {return obj.id; }
	);

	msg << gridPos;
}

void Grid::unpackMessage(Message& msg) {
	msg >> gridPos;	

	std::vector<Instance_Id> objs;
	unpackArray<Instance_Id>(msg,
		objs,
		[](net::message<NetMsgType>& msg)
		{ Instance_Id id; msg >> id; return id; }
	);
}

void Grid::RemoveAllObjects() {
	_go_vec.clear();
	_go_map.clear();
}

void Grid::AddObject(GameObjPtr go) {
	if (!go) return;

	_go_vec.push_back(GOObj{.id = go->id(), .go = go });
	_go_map.insert({ go->id(), go });
}

std::optional<GameObjPtr> Grid::FindObject(Instance_Id id) {
	auto it = _go_map.find(id);
	if(it == _go_map.end())
		return std::nullopt;

	return std::make_optional<GameObjPtr>(it->second);
}

bool Grid::RemoveObject(Instance_Id id) {
	auto it = _go_map.find(id);
	if (it == _go_map.end())
		return false;

	_go_map.erase(it);

	for (int i = 0; i < _go_vec.size(); i++) {
		if (_go_vec[i].id == id) {
			_go_vec.erase(_go_vec.begin() + i);
			break;
		}
	}

	return false;
}
