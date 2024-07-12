#include <algorithm>

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

void SceneManager::ApplyUpdate(GameObject* go, Message& msg) {
	//assume gop has been taken care of already

	SyncTarget st;
	while (msg.size() > 0) {
		msg >> st;
		GameObjectFactory::UnpackAs(st.class_id, st.diff, msg, go);
	}
}

bool SceneManager::NeedsUpdate(GameObject* go) {
	return !go->getSynchronizationTargets().empty();
}

Message SceneManager::POST(Instance_Id gid, GameObject* go) {
	GameObjectPost gop{
		.time = go->_updateTimes,
		.rootClassId = go->GetClassId()
	};
	gop.id.grid_id = gid;
	gop.id.inst_id = go->id();

	Message msg;
		msg.header.id = NetMsgType::GameObjectPost;
	
	go->packMessage(msg, DEFAULT_MsgDiff_EVERYTHING);
	msg << gop;

	return msg;
}

Message SceneManager::UPDATE(Instance_Id gid, GameObject* go) {
	GameObjectUpdate gop{ .time = go->_updateTimes };
		gop.id.grid_id = gid;
		gop.id.inst_id = go->id();
	
	Message msg;
		msg.header.id = NetMsgType::GameObjectUpdate;

	for (SyncTarget st : go->getSynchronizationTargets()) {
		GameObjectFactory::PackAs(st.class_id, st.diff, msg, go);
		msg << st;
	}

	go->clearSynchronizationTargets();

	msg << gop;

	return msg;
}

std::optional<Message>
	SceneManager::processMessage(Message& msg, GameObjectUpdate gou) {

	std::shared_ptr<Grid> grid;

	//if hte target grid dne, ignore current request, ask for the grid
	auto it = grids.find(gou.id.grid_id);
	if (it == grids.end()) {

		//because this can be triggered by any Update, need a cool down to avoid spamming
		//	the implimentation bellow does not account for mutiple grids
		static time_t lastSent = 0;
		time_t rn = GetTime();
		if (rn - lastSent < 200) return std::nullopt;
		lastSent = rn;

		//set the grid as the thing we're trying to request
		gou.id.inst_id = gou.id.grid_id;

		goto requestCompleteObject;
	}

	//confirmed grid exists localy
	grid = it->second;

	if (!gou.id.IsGrid()) {
		auto targ = grid->FindObject(gou.id.inst_id);
		
		//if missing target object, request it
		if(!targ)
			goto requestCompleteObject;

		//apply message content to object (is not a grid)
		ApplyUpdate(targ.value().get(), msg);
	} else //apply message content to object(is a grid)
		ApplyUpdate(grid.get(), msg);

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

	if (gop.id.IsGrid()) {
		std::shared_ptr<Grid> obj;
		
		//make new grid if necessary, reuse old one otherwise
		if (git == grids.end()) {
			obj = static_pointer_cast<Grid>(GameObjectFactory::GetInstance(gop.rootClassId));

			obj->setId(gop.id.grid_id);

			grids[gop.id.grid_id] = obj;
		} else {
			obj = git->second;
		}

		obj->_updateTimes = gop.time;

		//because this is post, can assume it is top class, so no need to cast
		obj->unpackMessage(msg, DEFAULT_MsgDiff_EVERYTHING);

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

		GameObjPtr obj;

		//if is a existing opject just update it to match
		// going with the assumption that theres no id conflict, were screwed if there is!
		auto op = git->second->FindObject(gop.id.inst_id);
		if (op)
			obj = op.value();
		else {
			obj = GameObjectFactory::GetInstance(gop.rootClassId);

			obj->setId(gop.id.inst_id);
		}

		obj->_updateTimes = gop.time;

		obj->unpackMessage(msg, DEFAULT_MsgDiff_EVERYTHING);

		//add to grid
		git->second->AddObject(obj);

		return std::nullopt;
	}
}
