#pragma once

#include <functional>

#include "NetCommon/eol_net.hpp"
#include "GameStructs.hpp"
#include "better-enums/enum.h"
#include "Game_common.hpp"

//basically server api
//the enum is used to map to its corrosponding struct. "corrospoonding" as in the names match(very closely at least).
//comments describe it form the receivers pov
BETTER_ENUM(BE_NetMsgType, uint8_t,
	
	/*0.
	* measures round trip time
	* client & server: if complete record the elapsed time, otherwise respond back
	*/
	Ping,
	
	/*1.
	* this dosent do anything, too much work to remove ir
	* server: 
	* is it someone in the queue ? or ready to start ?
	*/
	ConnectionStatus, 

	/*2.
	* request a GameObject id partition
	* client: a new valid partition
	* server: (not currently implimented) responds indicating a unused idpartition the server is free to reuse
	*/
	IDPartition,

	/*
	* instructions to resolve id collision
	* server & client: set the obj id to a new with the given id to 
	*/
	IDCorrection,

	//4. request resource by id. grid and object id supprted
	RequestById,

	//user tries to make a new grid at a location. if location is 
	// not allowed server corrects grid id to effectively rerout the user
	GridRequest,

	//6. game object update event. 
	GameObjectUpdate,

	//game object creation event (has full description)
	GameObjectPost
);
typedef BE_NetMsgType::_enumerated NetMsgType;
typedef net::message<NetMsgType> Message;

template<typename ELE, typename TARG = ELE>
void inline packArray(
	Message& msg,
	std::vector<ELE>& vec,
	std::function<TARG(ELE&)> getVal
) {
	static_assert(std::is_standard_layout<TARG>::value);

	for (auto rit = vec.rbegin(); rit != vec.rend(); ++rit)
		msg << getVal(*rit);

	msg << vec.size();
}

template<typename T>
void inline unpackArray(
	Message& msg,
	std::vector<T>& arr,
	std::function<T(Message&)> getVal
) {
	static_assert(std::is_standard_layout<T>::value);

	size_t len;
	msg >> len;

	arr.reserve(len + arr.size());

	for (size_t i = 0; i < len; i++)
		arr.push_back(getVal(msg));
}

//////////////////////////////////////////////////////////////////////////////
//	standarized server messages
//////////////////////////////////////////////////////////////////////////////

struct ID {
	Instance_Id grid_id;
	Instance_Id inst_id;

	bool IsGrid() {
		return grid_id == inst_id;
	}
};
static_assert(std::is_standard_layout<ID>::value);

struct IDPartition {
	Instance_Id min, max, nxt;
	
	IDPartition() : IDPartition(0,0,-1) {}
	IDPartition(Instance_Id mi, Instance_Id mx, Instance_Id nx) : min(mi), max(mx), nxt(nx) {}

	Instance_Id getNext() {
		return nxt++;
	}

	bool withinRange(Instance_Id num) {
		return num >= min && num <= max;
	}

	bool IsBad() {
		return nxt < min || nxt > max;
	}
};
static_assert(std::is_standard_layout<IDPartition>::value);
static IDPartition LOCAL_PARITION = IDPartition();

struct ConnectionStatus {
	Instance_Id clientId;
	bool isQueue;
	ConnectionStatus() : isQueue(false) {}

};
static_assert(std::is_standard_layout<ConnectionStatus>::value);

struct Ping {
	time_t sent = -1, received = -1;
	
	Ping() {}

	bool isComplete()	{ return sent > 0 && received > 0; }
	time_t getTime()	{ return received - sent; }
	time_t tag();
};
static_assert(std::is_standard_layout<Ping>::value);

struct IDCorrection {
	ID formerID;
	Instance_Id newId;
};
static_assert(std::is_standard_layout<IDCorrection>::value);

struct RequestById {
	ID id;
};
static_assert(std::is_standard_layout<RequestById>::value);

struct GridRequest {
	GridRequest() : pos(gs::Vec2::BAD) {}

	gs::Vec2 pos;
};
static_assert(std::is_standard_layout<GridRequest>::value);

//////////////////////////////////////////////////////////////////////////////
//	standarized gameobject messages
// * see Gameobject & GameObjectFactory.
//////////////////////////////////////////////////////////////////////////////

struct GameObjectUpdate {
	ID id;
	time_t time;
	//[Classes]
};
static_assert(std::is_standard_layout<GameObjectUpdate>::value);

struct GameObjectPost { //see server.cpp message handler for setup
	ID id;
	time_t	time;
	//[Classes]
};
static_assert(std::is_standard_layout<GameObjectPost>::value);

struct Classes {
	std::vector<Class_Id> class_ids;

	Classes() = default;
	Classes(std::vector<Class_Id> ids) : class_ids(ids) {}
	Classes(net::message<NetMsgType>& msg) {
		Unpack(msg);
	}

	void Pack(net::message<NetMsgType>& msg) {
		packArray<Class_Id, Class_Id>(msg, class_ids, [](Class_Id cid) { return cid; });
	}

	void Unpack(net::message<NetMsgType>& msg) {
		unpackArray<Class_Id>(msg, class_ids, [](net::message<NetMsgType>& msg) { Class_Id cid; msg >> cid; return cid; });
	}

	static Classes GetClasses(net::message<NetMsgType>& msg) {
		Classes ret;
		ret.Unpack(msg);
		return ret;
	}
};

struct GameObjectGridChange : GameObjectUpdate{
	Instance_Id current_grid_id;
};
static_assert(std::is_standard_layout<GameObjectPost>::value);
