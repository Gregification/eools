#pragma once

#include <type_traits>
#include <functional>
#include <unordered_set>

#include "GameStructs.hpp"
#include "better-enums/enum.h"

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

typedef net::message<NetMsgType>
	Message;

//interface 
struct Messageable {
	virtual void packMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) = 0;
	virtual void unpackMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) = 0;
};

//////////////////////////////////////////////////////////////////////////////
//	standarized syncing
//////////////////////////////////////////////////////////////////////////////

struct UpdateTime {
	time_t lastUpdate;
	time_t lastFixedUpdate;
};
static_assert(std::is_standard_layout<UpdateTime>::value);

//indicates the changes involved(for both packing & unpacking)
struct SyncTarget {
	Class_Id class_id;
	MsgDiffType diff;

	bool operator==(const SyncTarget& other) const {
		return class_id == other.class_id && diff == other.diff;
	}
};
static_assert(std::is_standard_layout<SyncTarget>::value);

struct SyncTargetHash {
	auto operator()(const SyncTarget& sr) const -> uint16_t { //easier if hash size matches the structs size
		return std::bit_cast<std::uint16_t>(sr);
	}
};

typedef std::unordered_set<SyncTarget, SyncTargetHash>
	SyncCollection; //the collection type that synch requests are buffered in

template<typename ELE, typename TARG = ELE>
void packArray(
	Message& msg,
	std::vector<ELE>& vec,
	std::function<TARG(ELE&)> getVal = [](ELE& e) -> TARG { return e; }
) {
	static_assert(std::is_standard_layout<TARG>::value);

	for (auto rit = vec.rbegin(); rit != vec.rend(); ++rit)
		msg << getVal(*rit);

	msg << vec.size();
}

template<typename T>
void unpackArray(
	Message& msg,
	std::vector<T>& arr,
	std::function<T(Message&)> getVal = [](Message& msg) -> T { T a; msg << a; return a; }
) {
	if (msg.size() < sizeof(size_t)) return;

	size_t len;
	msg >> len;

	//reserving ahead of time actually seems to give +~4 fps ... sad that its come to this...
	int off = len - (arr.capacity() - arr.size());
	if(off > 0)
		arr.reserve(off);

	for (size_t i = 0; i < len; i++)
		arr.push_back(getVal(msg));
}

//////////////////////////////////////////////////////////////////////////////
//	standarized content
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
	
	IDPartition() : IDPartition(0,0,1) {}
	IDPartition(Instance_Id mi, Instance_Id mx, Instance_Id nx) : min(mi), max(mx), nxt(nx) {}

	static IDPartition LOCAL_PARITION;

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
	GridRequest() : pos(gs::Vec2()) {}
	GridRequest(gs::Vec2 p) : pos(p) {}

	gs::Vec2 pos;
};
static_assert(std::is_standard_layout<GridRequest>::value);

//////////////////////////////////////////////////////////////////////////////
//	standarized gameobject messages
// * see Gameobject & GameObjectFactory.
//////////////////////////////////////////////////////////////////////////////

struct GameObjectUpdate {
	ID id;
	UpdateTime time;
};
static_assert(std::is_standard_layout<GameObjectUpdate>::value);

struct GameObjectPost { //see server.cpp message handler for setup
	ID id;
	UpdateTime time;
	Class_Id rootClassId;
};
static_assert(std::is_standard_layout<GameObjectPost>::value);

struct GameObjectGridChange : GameObjectUpdate{
	Instance_Id current_grid_id;
};
static_assert(std::is_standard_layout<GameObjectPost>::value);
