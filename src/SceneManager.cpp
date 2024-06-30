#include "SceneManager.hpp"

std::unordered_map<Instance_Id, std::shared_ptr<Grid>> SceneManager::grids;

std::shared_ptr<Grid> SceneManager::getGrid(Vec2 pos) {
	if(!pos.IsBad())
		for (auto& [id, g] : grids)
			if ((g->transform.position - pos).magnitudeSquared() < MIN_GRID_SEPERATION_SQUARED)
				return g;

	std::shared_ptr<Grid> grid(new Grid);
	grid->transform.position = pos;
	grid->setId(LOCAL_PARITION.getNext());
	return grid;
}

std::shared_ptr<Grid> SceneManager::getGrid(Instance_Id id) {
	if (id == BAD_ID || grids.find(id) == grids.end())
		return std::shared_ptr<Grid>(nullptr);

	return std::move(grids.find(id)->second);
}

void SceneManager::correctID(IDCorrection idc) {
	auto grid = getGrid(idc.formerID.gridId);
	if (!grid) return;
	
	
}

void SceneManager::processMessage(net::message<NetMsgType> msg, std::function<void(const net::message<NetMsgType>&)> Send) {
	assert(msg.header.id == NetMsgType::GameObjectUpdate);


}

std::optional<std::shared_ptr<GameObject>> SceneManager::find(Instance_Id id) {
	return std::nullopt;
}