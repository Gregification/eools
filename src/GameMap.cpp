#include "GameMap.hpp"


std::shared_ptr<Grid> GameMap::makeGrid(Vec2 pos) {
	for (auto& [id, g] : grids)
		if ((g->transform.position - pos).magnitudeSquared() < MIN_GRID_SEPERATION_SQUARED)
			return g;

	std::shared_ptr<Grid> grid(new Grid);
	grid->transform.position = pos;
	grid->id = LOCAL_PARITION.getNext();
	return grid;
}

std::shared_ptr<Grid> GameMap::getGrid(id_t id) {
	if (id == BAD_ID || grids.find(id) == grids.end())
		return std::shared_ptr<Grid>(nullptr);

	return std::move(grids.find(id)->second);
}

void GameMap::Draw(Canvas&, Transformation_2D& transform) {
	for (auto& [id, grid] : grids) {

	}
}

void GameMap::correctID(IDCorrection idc) {
	auto grid = getGrid(idc.formerId.gridId);
	if (!grid) return;

	
}

void GameMap::processMessage(net::message<NetMsgType> msg, std::function<void(const net::message<NetMsgType>&)> Send) {
	assert(msg.header.id == NetMsgType::GameObjectUpdate);

	auto gou = GameObjectUpdate();
	msg >> gou;

	auto grid = getGrid(gou.objectID.gridId);
	if (!grid) { // if local doesn't know grid exists
		//assume grid is valid and all is good

		//register grid localy
		grid = std::shared_ptr<Grid>(new Grid);
		grid->id = gou.objectID.gridId;
		grids.insert({gou.objectID.gridId, grid});

		//if the intention was not to tell of the creation of a new grid, then ask for more info on the grid
		if (gou.objectID.targetType != ID::GRID) {
			auto msg = net::message<NetMsgType>();
			msg.header.id = NetMsgType::RequestById;

			auto gc = RequestById();
				gc.targetId.targetType = ID::GRID;
				gc.targetId.id = gou.objectID.gridId;

			Send(msg);
		}
	}

	if (gou.transformOnly) {
		Transform tf = {};
		msg >> tf;

		switch (gou.objectID.targetType) {
			case ID::ID_TYPE::GRID: {
				
			} break;
			case ID::ID_TYPE::PLAYER: {

			} break;
			case ID::ID_TYPE::OBJECT: {

			} break;
		}
	}
	else switch (gou.objectID.targetType) {
		case ID::ID_TYPE::GRID: {
				
			} break;
		case ID::ID_TYPE::PLAYER: {

			} break;
		case ID::ID_TYPE::OBJECT: {

			} break;
	}
}

std::shared_ptr<GameObject> GameMap::find(id_t) {
	return std::shared_ptr<GameObject>(nullptr);
}