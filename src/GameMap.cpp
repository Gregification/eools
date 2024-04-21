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
	grid->id = LOCAL_PARITION.getNext();
	return grid;
}

std::shared_ptr<Grid> GameMap::getGrid(inst_id id) {
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

	auto gou = GameObjectUpdate();
	msg >> gou;

	std::shared_ptr<GameObject> target = find(gou.objectID);

	if (!target) { //if local has no record of the object
		//set up a new instance
		target = GameObjectFactory::getInstance(gou.objectID.classId);
		
		//check if the grid even exists
		//grids are a special case that have to be set up manualy
		if (grids.find(gou.objectID.gridId) == grids.end()) { //if grid DNE
			std::shared_ptr<Grid> grid;

			if (gou.objectID.targetType == ID::GRID) //if intention was to introduce a new grid
				grid = std::dynamic_pointer_cast<Grid>(target); //then the target must be a grid
			else
				grid = std::shared_ptr<Grid>(new Grid); //otherwise improvise a new grid
			
			//either case, locally register the grid
			grids.insert( {gou.objectID.gridId, grid} );

			//send upate after its registered localy so itll work for multi 
			// threaded updates if I ever get to that
			if (gou.objectID.targetType == ID::GRID) { //if the grid wasent the target, request a update for it
				auto gc = RequestById();
					gc.targetID			= gou.objectID;
					gc.transformOnly	= false;

				auto msg = net::message<NetMsgType>();
					msg.header.id		= NetMsgType::RequestById;

				msg << gc;
				Send(msg);
			}
		}
	}

	//apply update
	if (gou.transformOnly)		//if updating transform only
		msg >> (target->transform);
	else						//otherwise object specific unpacking needed
		target->unpackMessage(msg);
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