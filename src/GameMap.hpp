#pragma once

#include "Grid.hpp"

/**
* map of everything. servers copy should have all there is to be
*/
class GameMap {
public:
	GameMap() : grids(){

	}
	~GameMap() {}

public:
	std::shared_ptr<Grid> makeGrid(Vec2);
	inline std::shared_ptr<Grid> getGrid(id_t);

public:
	std::unordered_map<id_t, std::shared_ptr<Grid>> grids;

};