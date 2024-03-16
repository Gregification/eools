#pragma once

#include <cstdint>
#include <functional>
#include <eol_net.hpp>

#include "Game_common.hpp"

enum class NetMsgType : id_t {
	Ping,
	
	//request a GameObject id partition
	IDPartition,

	//correction to resolve id collision. 
	//broadcast
	IDCorrection,

	//starter pack of info when joining the game
	StarterPacket,

	//request resource by id. grid and object id supprted
	RequestById,

	//game object update event. 
	//broadcast
	GameObjectUpdate,

	//new game object event
	GameObjectDeclaration,

	GridChange
};

template<typename ELE, typename TARG>
void packArray(
		net::message<NetMsgType>& msg,
		const ELE* arr,
		const size_t len,
		const TARG* (*getVal)(const ELE*, size_t) = (const ELE * arr, size_t i){ return static_cast<TARG>(arr[i]); }
	){
	static_assert(std::is_standard_layout<TARG>::value);

	for(id_t i = len - 1; i >= 0; i--)
		msg << getVal(arr, i);

	msg << len;
}

template<typename T>
void unpackArray(
		net::message<NetMsgType>& msg,
		std::vector<T>& arr,
		T (*getVal)(net::message<NetMsgType>&)
	){
	static_assert(std::is_standard_layout<T>::value);
	id_t len = 0;
	msg >> len;

	arr.reserve(len + arr.size());

	for (id_t i = 0; i < len; i++)
		arr.push_back(getVal(msg));
}

//////////////////////////////////////////////////////////////////////////////
//	standarized server messages
//////////////////////////////////////////////////////////////////////////////

struct IDPartition {

	id_t min, max, nxt;
	
	IDPartition() : IDPartition(0,0,0) {}
	IDPartition(id_t mi, id_t mx, id_t nx) : min(mi), max(mx), nxt(nx) {}

	id_t getNext() {
		return nxt++;
	}

	bool withinRange(id_t num) {
		return num >= min && num <= max;
	}
};
static_assert(std::is_standard_layout<struct IDPartition>::value);
static struct IDPartition LOCAL_PARITION = {};

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
		flag_byGridId		= 1 << 0,
		flag_forTransform	= 1 << 1;
	id_t id, flags;

	bool isLookingForTransform() const {
		return flags & flag_forTransform;
	}

	bool isLookingForGrid() {

	}
};
static_assert(std::is_standard_layout<struct RequestById>::value);

struct StarterPacket {
	id_t gridId;
};
static_assert(std::is_standard_layout<struct StarterPacket>::value);
//////////////////////////////////////////////////////////////////////////////
//	standarized gameobject messages
//////////////////////////////////////////////////////////////////////////////

//nothing. the objects sort themselves. see Gameobject class.