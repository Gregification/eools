#include <span>
#include "Grid.hpp"

//gold mine https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf
//https://www.youtube.com/watch?v=mD3cbXu3ZBE


void Grid::Draw(Canvas& c, Transformation_2D trf) {
	
}

void Grid::packMessage(Message& msg, MsgDiffType) {
	packArray<GOObj> (
		msg,
		_go_vec,
		[](Message& m, GOObj& obj) { m << obj.id; }
	);

	msg << gridPos;
}

void Grid::unpackMessage(Message& msg, MsgDiffType) {
	msg >> gridPos;	
	
	std::vector<Instance_Id> objs;
	unpackArray<Instance_Id>(msg, objs);
	//TODO: add the id's if they dont exist
}

void Grid::RemoveAllObjects() {
	_go_vec.clear();
	_go_map.clear();
}

//TODO: bvh setup https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf
//		also link it to the renderer somehow
GameObjPtr Grid::ObjectAt(const Vec2& gridPos) const {

	for(const auto& v : _go_vec)
		if(v.go 
			&& v.go->getAABB().containsPoint(gridPos)
			&& v.go->containsPoint(static_cast<Vec2_f>(gridPos - v.go->transform.position))
			)
			return GameObjPtr(v.go);

	return GameObjPtr();
}

std::vector<GameObjPtr> Grid::ObjectsWithin(const gs::Rectangle& gridArea) const {
	std::vector<GameObjPtr> ret{};

	for (const auto& v : _go_vec)
		if (v.go
			&& gridArea.overlaps(v.go->getAABB())
			&& gridArea.overlaps(v.go->verticies)
			)
			ret.push_back(v.go);

	return ret;
}

const std::vector<Grid::GOObj> Grid::getObjVec() const {
	return _go_vec;
}

void Grid::AddObject(GameObjPtr go) {
	if (!go) return;

	_go_vec.push_back(GOObj{.id = go->id(), .go = go });
	_go_map.insert({ go->id(), go });
}

std::optional<GameObjPtr> Grid::FindObject(Instance_Id id) {
	auto it = _go_map.find(id);
	if (it == _go_map.end())
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
