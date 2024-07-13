#pragma once

#include <cmath>
#include <memory>

#include "NetGameObject.hpp"
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

	/****************************************************************
	* game
	****************************************************************/

	/*gets grid at position, or makes one if couldnt be found*/
	std::shared_ptr<Grid> GetGrid(Vec2);
	/*gets grid at position, if it exists*/
	std::optional<std::shared_ptr<Grid>> GridAt(Vec2);

	std::optional<GameObjPtr> find(ID);

	void processGrid(
		Grid*,
		time_t,
		std::function<void(Message)>,
		std::function<bool(GameObject*)>
	);

	/****************************************************************
	* netowrk
	****************************************************************/
	void CorrectID(IDCorrection);
	void ApplyUpdate(GameObject*, Message&);

	bool NeedsUpdate(GameObject*);

	Message POST(Instance_Id grid_id, GameObject* obj);
	Message UPDATE(Instance_Id grid_id, GameObject* obj);

	std::optional<Message> processMessage(Message&, GameObjectUpdate);
	std::optional<Message> processMessage(Message&, GameObjectPost);
};