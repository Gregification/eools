#include "GameMap.hpp"

std::shared_ptr<Grid> GameMap::makeGrid(Vec2 pos) {
	auto grid = std::make_shared<Grid>();

	return grid;
}

std::shared_ptr<Grid> GameMap::getGrid(id_t id) {
	return grids.at(id);
}