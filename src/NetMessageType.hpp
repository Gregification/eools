#pragma once

#include <cstdint>
#include <chrono>
#include <functional>

#include "NetCommon/eol_net.hpp"
#include "Game_common.hpp"
#include "GameStructs.hpp"

//basically server api
//the enum is used to map to its corrosponding struct. "corrospoonding" as in the names match(very closely at least).

enum class NetMsgType : uint16_t {
	Ping,
	
	//is it someone in the queue? or ready to start?
	ConnectionStatus,

	//2. request a GameObject id partition
	IDPartition,

	//correction to resolve id collision. 
	//broadcast
	IDCorrection,

	//4. request resource by id. grid and object id supprted
	RequestById,

	//user tries to make a new grid at a location. if location is 
	// not allowed server corrects grid id to effectively rerout the user
	GridRequest,

	//6. game object update event. 
	GameObjectUpdate,

	//game object creation event (has full description)
	GameObjectPost,
};

template<typename ELE, typename TARG = ELE>
void packArray(
		net::message<NetMsgType>& msg,
		std::vector<ELE>& vec,
		TARG& (*getVal)(ELE&)
	){
	static_assert(std::is_standard_layout<TARG>::value);

	size_t len = vec.size();
	for(size_t i = 0; i < len; i++)
		msg << getVal(vec[i]);

	msg << len;
}

template<typename T>
void unpackArray(
		net::message<NetMsgType>& msg,
		std::vector<T>& arr,
		T (*getVal)(net::message<NetMsgType>&)
	){
	static_assert(std::is_standard_layout<T>::value);
	size_t len = 0;

	msg >> len;

	arr.reserve(len + arr.size());

	for (size_t i = 0; i < len; i++)
		arr.push_back(getVal(msg));
}

//////////////////////////////////////////////////////////////////////////////
//	standarized server messages
//////////////////////////////////////////////////////////////////////////////

struct ID {
	enum ID_TYPE : uint8_t {
		GRID,
		PLAYER,
		OBJECT,
		BAD_TYPE
	};
	ID() : instanceId(BAD_ID), gridId(BAD_ID), targetType(BAD_TYPE), classId(BAD_CLASS_ID) {}

	bool isBad() {
		return	ID_TYPE::BAD_TYPE == targetType
			||	BAD_CLASS_ID == classId
			||	BAD_ID == gridId
			||	BAD_ID == instanceId;
	}

	ID_TYPE targetType;
	Instance_Id gridId;
	Instance_Id instanceId;
	Class_Id classId;
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

	bool isBad() {
		return nxt < min || nxt > max;
	}
};
static_assert(std::is_standard_layout<IDPartition>::value);
static IDPartition LOCAL_PARITION = IDPartition();

struct ConnectionStatus {
	Instance_Id clientId;
	bool isQueue;
	ConnectionStatus() : isQueue(false), clientId(getIdFromPartition(LOCAL_PARITION)) {}

	static Instance_Id getIdFromPartition(const IDPartition& part) {
		return part.min / STD_PARTITION_SIZE;
	}
};
static_assert(std::is_standard_layout<ConnectionStatus>::value);

struct Ping {
	time_t sent = -1, received = -1;
	
	Ping() {}

	bool isComplete()	{ return sent > 0 && received > 0; }
	time_t getTime()	{ return received - sent; }
	long long tag() { 
		using namespace std::chrono;
		time_t rn = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		if (sent <= 0)
			sent = rn;
		else
			received = rn;

		return getTime();
	}
};
static_assert(std::is_standard_layout<Ping>::value);

struct IDCorrection {
	ID formerID;
	Instance_Id newId;
};
static_assert(std::is_standard_layout<IDCorrection>::value);

struct RequestById {
	ID targetID;
	bool transformOnly;
};
static_assert(std::is_standard_layout<RequestById>::value);

struct GridRequest {
	GridRequest() : pos(0,0) {}

	gs::Vec2 pos;
};
static_assert(std::is_standard_layout<GridRequest>::value);


//////////////////////////////////////////////////////////////////////////////
//	standarized gameobject messages
// * see Gameobject & GameObjectFactory.
//////////////////////////////////////////////////////////////////////////////

struct GameObjectUpdate {
	Instance_Id grid_id;
	Instance_Id inst_id;
	time_t		time;

	static struct Classes {
		std::vector<Class_Id> as;
	};

};
static_assert(std::is_standard_layout<GameObjectUpdate>::value);

struct GameObjectPost {

};
static_assert(std::is_standard_layout<GameObjectPost>::value);

struct GameObjectGridChange {

};
static_assert(std::is_standard_layout<GameObjectPost>::value);
