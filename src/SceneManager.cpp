#include "SceneManager.hpp"

std::unordered_map<Instance_Id, std::shared_ptr<Grid>> SceneManager::grids;

std::shared_ptr<Grid> SceneManager::GetGrid(Vec2 pos) {
	auto g = GridAt(pos);
	if (g) return g.value();

	std::shared_ptr<Grid> grid(new Grid);
	grid->transform.position = pos;
	grid->setId(LOCAL_PARITION.getNext());
	return grid;
}

std::optional<std::shared_ptr<Grid>> SceneManager::GridAt(Vec2 pos) {
	if (!pos.IsBad())
		for (auto& [id, g] : grids)
			if ((g->transform.position - pos).magnitudeSquared() < MIN_GRID_SEPERATION_SQUARED)
				return g;

	return std::nullopt;
}

void SceneManager::correctID(IDCorrection idc) {
	
}

std::optional<net::message<NetMsgType>> 
	SceneManager::processMessage(net::message<NetMsgType> msg, GameObjectUpdate gou) {
	Classes cls(msg);

	return std::nullopt;
}

std::optional<net::message<NetMsgType>> 
	SceneManager::processMessage(net::message<NetMsgType> msg, GameObjectPost gop) {
	Classes cls(msg);

	GameObjectFactory::getInstance(cls.class_ids[0]);

	return std::nullopt;
}

std::optional<std::shared_ptr<GameObject>> SceneManager::find(Instance_Id id) {
	return std::nullopt;
}