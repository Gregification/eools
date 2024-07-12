#include <span>
#include "Grid.hpp"

//gold mine https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf
//https://www.youtube.com/watch?v=mD3cbXu3ZBE

void Grid::Update(time_t currTime) {
	for (int i = 0; i < _go_vec.size(); i++) {
		if (_go_vec[i].go) {
			GameObject* gp = _go_vec[i].go.get();

			gp->Update(GetDT(currTime - gp->lastUpdate));
			gp->_updateTimes.lastUpdate = currTime;
		}
	}
}

void Grid::FixedUpdate(time_t currTime) {
	for (int i = 0; i < _go_vec.size(); i++) {
		if (_go_vec[i].go) {
			GameObject* gp = _go_vec[i].go.get();

			gp->FixedUpdate(GetDT(currTime - gp->lastUpdate));
			gp->_updateTimes.lastFixedUpdate = currTime;
		}
	}
}

void Grid::FixedUpdate_w_messaging(time_t currTime, std::function<void(GameObject*)> godo) {
	for (int i = 0; i < _go_vec.size(); i++) {
		if (_go_vec[i].go) {
			GameObject* gp = _go_vec[i].go.get();
		
			gp->FixedUpdate(GetDT(currTime - gp->lastUpdate));
			gp->lastUpdate = currTime;

			godo(gp);
		}
	}
}

void Grid::Draw(Canvas& c, Transformation_2D& trf) const {
	c.DrawText(20, 20, "drawing grid w/ id: " + std::to_string(id()));
	c.DrawText(20, 30, "# children: " + std::to_string(_go_vec.size()));
	int y = 30;
	for (auto& v : _go_vec) {
		c.DrawText(25, y+=10, "id: " + std::to_string(v.id) + "?" + (v.go ? ("exists@(" + std::to_string(v.go->transform.position.x) + "," + std::to_string(v.go->transform.position.y) + ")") : "dne"));
		if (!v.go) continue;

		v.go->Draw(c, trf);
	}
}

void Grid::packMessage(Message& msg, MsgDiffType) {
	packArray<GOObj, Instance_Id> (
		msg,
		_go_vec,
		[](GOObj& obj) {return obj.id; }
	);

	msg << gridPos;
}

void Grid::unpackMessage(Message& msg, MsgDiffType) {
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
