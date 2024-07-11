#include "SceneManager.hpp"

#include "GameObjectFactory.hpp"

std::unordered_map<Instance_Id, std::shared_ptr<Grid>> SceneManager::grids;

std::shared_ptr<Grid> SceneManager::GetGrid(Vec2 pos) {
	auto g = GridAt(pos);
	if (g) return g.value();

	std::shared_ptr<Grid> grid(new Grid);
	grid->transform.position = pos;
	grid->setId(IDPartition::LOCAL_PARITION.getNext());

	grids[grid->id()] = grid;

	return grid;
}

std::optional<std::shared_ptr<Grid>> SceneManager::GridAt(Vec2 pos) {
	if (!pos.IsBad())
		for (auto& [id, g] : grids)
			if ((g->transform.position - pos).magnitudeSquared() < MIN_GRID_SEPERATION_SQUARED)
				return g;

	return std::nullopt;
}

std::optional<GameObjPtr> SceneManager::find(ID id) {
	std::shared_ptr<Grid> grid;
	{
		auto git = grids.find(id.grid_id);
		if (git == grids.end()) return std::nullopt;
		grid = git->second;
	}

	if (id.IsGrid()) {
		return grid;
	} else {
		return grid->FindObject(id.inst_id);
	}
}

void SceneManager::CorrectID(IDCorrection idc) {
	auto op = find(idc.formerID);
	if (op) {
		op.value()->setId(idc.newId);
	}
}

void SceneManager::ApplyClasses(GameObjPtr go, Classes& clas, Message& msg) {
	auto* ptr = go.get();
	for (Class_Id cid : clas.class_ids) {
		GameObjectFactory::UnpackAs(cid, msg, ptr);
	}
}

std::optional<Message>
	SceneManager::processMessage(Message& msg, GameObjectUpdate gou) {
	Classes clas(msg);

	std::shared_ptr<Grid> grid;

	auto it = grids.find(gou.id.grid_id);
	if (it == grids.end()) { //if the target grid dne

		//to avoid spamming mutiple grid requests. does not account for mutiple grids
		static time_t lastSent = 0;
		time_t rn = GetTime();
		if (rn - lastSent < 200) return std::nullopt;
		lastSent = rn;

		gou.id.inst_id = gou.id.grid_id; //get the grid instead of what ever the update was for
		goto requestCompleteObject;
	}

	grid = it->second;
	if (!gou.id.IsGrid()) {
		auto targ = grid->FindObject(gou.id.inst_id);
		
		if(!targ)
			goto requestCompleteObject;

		ApplyClasses(targ.value(), clas, msg);
	} else 
		ApplyClasses(grid, clas, msg);

	return std::nullopt;


	requestCompleteObject:
	
	RequestById content{ .id = gou.id };

	msg.body.clear();
	msg.header.id = NetMsgType::RequestById;
	msg << content;

	return std::make_optional(msg);
}

std::optional<Message>
	SceneManager::processMessage(Message& msg, GameObjectPost gop) {

	auto git = grids.find(gop.id.grid_id);
	Classes clas(msg);

	if (gop.id.IsGrid()) {
		std::shared_ptr<Grid> obj;
		
		//make new grid if necessary, reuse old one otherwise
		if (git == grids.end()) {
			obj = dynamic_pointer_cast<Grid>(GameObjectFactory::GetInstance(clas.class_ids[0]));
		} else {
			obj = git->second;
		}

		obj->lastUpdate = obj->lastUpdate_fixed = gop.time;
		obj->setId(gop.id.grid_id);

		ApplyClasses(obj, clas, msg);
		
		grids[gop.id.grid_id] = obj;

		return std::nullopt;
	}
	else {

		//if grid does not exist
		if (git == grids.end()) { 
			//ignore current request, get the grid we're missing
			RequestById response;
				response.id.grid_id = response.id.inst_id = gop.id.grid_id;

			Message res;
			res << response;
			return std::optional<net::message<NetMsgType>>{res};
		}

		//just going ot be positive and assume its not a overlapping id
		// if it is then its over, the problem could be anywhere on server or client
		auto obj = GameObjectFactory::GetInstance(clas.class_ids[0]);
		obj->setId(gop.id.inst_id);

		ApplyClasses(obj, clas, msg);

		git->second->AddObject(obj);

		//synch obj with local grid. 
		//    - this is not thread safe <- update can potentially access other objs
		//    - can result in negative time updates (oh well)
		obj->Update(GetDT(gop.time - git->second->lastUpdate));
		obj->Update(GetDT(gop.time - git->second->lastUpdate_fixed));

		return std::nullopt;
	}
}
