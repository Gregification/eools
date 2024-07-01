#pragma once

#include <cmath>
#include <memory>

#include "GameObjects/Grid.hpp"

#define MIN_GRID_SEPERATION 5'000'000.0
#define MIN_GRID_SEPERATION_SQUARED MIN_GRID_SEPERATION * MIN_GRID_SEPERATION

/**
* another boiler plate layer.
*	provides utils to coordinate local game status with remote, for both client and server.
* 
*   its up the client or server to use these utils.
*/
namespace SceneManager {
	using namespace gs;

	extern std::unordered_map<Instance_Id, std::shared_ptr<Grid>> grids;

	/*gets grid at position, or makes one if couldnt be found*/
	std::shared_ptr<Grid> GetGrid(Vec2);
	/*gets gris at position, if it exists*/
	std::optional<std::shared_ptr<Grid>> GridAt(Vec2);

	std::optional<std::shared_ptr<GameObject>> find(Instance_Id);

	void correctID(IDCorrection);
	std::optional<net::message<NetMsgType>> processMessage(net::message<NetMsgType>, GameObjectUpdate);
	std::optional<net::message<NetMsgType>> processMessage(net::message<NetMsgType>, GameObjectPost);
};