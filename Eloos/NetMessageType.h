#pragma once

#include <cstdint>
#include "GameStructs.hpp"

#define SERVER_PORT 60000
#define STD_PARTITION_SIZE 10000000 //10 mill => ~200 unique parts

enum class NetMsgType : uint32_t {
	Ping,
	
	//request a GameObject id partition
	IDPartition,

	//correction to resolve id collision. 
	//broadcast
	IDCorrection,

	//request resource by id. grid and object id supprted
	RequestById,

	//game object update event. 
	//broadcast
	GameObjectUpdate,

	GridChange
};

//////////////////////////////////////////////////////////////////////////////
//	standarized server messages
//////////////////////////////////////////////////////////////////////////////

struct IDPartition {
	id_t min, max, nxt;
	id_t getNext() {
		return nxt++;
	}

	bool withinRange(id_t num) {
		return num >= min && num <= max;
	}
};
static_assert(std::is_standard_layout<struct IDPartition>::value);

struct Ping {
	id_t sent = 0, received = 0;
	bool isComplete()	{ return sent && received; }
	void tagSent()		{ sent = std::time(0); }
	void tagReceived()	{ received = std::time(0); }
};
static_assert(std::is_standard_layout<struct Ping>::value);

struct IDCorrection {
	id_t formerId, newId;
};
static_assert(std::is_standard_layout<struct IDCorrection>::value);

struct RequestById {
	static const id_t
		flag_GameObject = 1,
		flag_Grid		= 1 << 1,
		flag_All		= 1 << 2;
	id_t id, flags;
};
static_assert(std::is_standard_layout<struct RequestById>::value);

//////////////////////////////////////////////////////////////////////////////
//	standarized gameobject messages
//////////////////////////////////////////////////////////////////////////////

//nothing. the objects sort themselves. see Gameobject class.