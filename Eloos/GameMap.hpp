#pragma once

#include "Grid.hpp"

/**
* map of everything. servers copy should have all there is to be
*/
class GameMap {
public:
	GameMap() : grids(5){

	}
	~GameMap() {}

public:
	std::vector<std::shared_ptr<Grid>> grids;

	std::shared_ptr<Grid> makeGrid(Vec2 pos) {
		auto grid = std::make_shared<Grid>();

		return grid;
	}
};