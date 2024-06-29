#include "GameMap.hpp"

#ifdef DrawText
#undef DrawText
#endif

std::shared_ptr<Grid> GameMap::getGrid(Vec2 pos) {
	for (auto& [id, g] : grids)
		if ((g->transform.position - pos).magnitudeSquared() < MIN_GRID_SEPERATION_SQUARED)
			return g;

	std::shared_ptr<Grid> grid(new Grid);
	grid->transform.position = pos;
	grid->setId(LOCAL_PARITION.getNext());
	return grid;
}

std::shared_ptr<Grid> GameMap::getGrid(Instance_Id id) {
	if (id == BAD_ID || grids.find(id) == grids.end())
		return std::shared_ptr<Grid>(nullptr);

	return std::move(grids.find(id)->second);
}

void GameMap::Draw(ftxui::Canvas& c, Transformation_2D& transform) {
	for (auto& [id, grid] : grids) {
		c.DrawText(c.width() / 2, c.height() / 2, "drawing grid");
	}
}

void GameMap::correctID(IDCorrection idc) {
	auto grid = getGrid(idc.formerID.gridId);
	if (!grid) return;
	
	
}

void GameMap::processMessage(net::message<NetMsgType> msg, std::function<void(const net::message<NetMsgType>&)> Send) {
	assert(msg.header.id == NetMsgType::GameObjectUpdate);


}

std::shared_ptr<GameObject> GameMap::find(ID id) {
	auto found = grids.find(id.gridId);
	if (found != grids.end()) {
		std::shared_ptr<Grid> grid = found->second;

		if (id.targetType == ID::GRID || !grid)
			return grid;

		for (auto& [gid, go] : grid->gameObjects)
			if (gid == id.instanceId)
				return go;
	}

	return nullptr;
}