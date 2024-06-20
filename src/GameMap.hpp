#pragma once

#include <cmath>
#include "GameObjects/Grid.hpp"

/**
* map of everything. servers copy should have all there is to be.
*	client copies just cache what ever it receives
* 
* this class is a special case that abstracts message management from server & client
*/
class GameMap {
public:
	GameMap() {

	}
	~GameMap() {}
	
	inline static const double MIN_GRID_SEPERATION_SQUARED =  std::pow(5'000'000.0f, 2);

public:
	std::shared_ptr<Grid> getGrid(Vec2);
	std::shared_ptr<Grid> getGrid(inst_id);

	Grid mapGrid;

	void Draw(Canvas&, Transformation_2D& transform);

	std::shared_ptr<GameObject> find(ID);

	void correctID(IDCorrection);
	void processMessage(net::message<NetMsgType>, std::function<void(const net::message<NetMsgType>&)> Send);

public:
	std::unordered_map<inst_id, std::shared_ptr<Grid>> grids;

};