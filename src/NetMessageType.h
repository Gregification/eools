#pragma once

#include <cstdint>
#include <chrono>
#include <functional>
#include "NetCommon/eol_net.hpp"

#include "Game_common.hpp"

enum class NetMsgType : uint16_t {
	Ping,
	
	//is it someone in the queue? or ready to start?
	ConnectionStatus,

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

	for(size_t i = len - 1; i >= 0; i--)
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
	enum ID_TYPE : uint16_t {
		GRID,
		USER,
		OBJECT
	};
	ID() : idNum(NAN), gridId(NAN), targetType(GRID) {}

	ID_TYPE targetType;
	id_t gridId;
	id_t idNum;
};
static_assert(std::is_standard_layout<struct ID>::value);

struct IDPartition {
	id_t min, max, nxt;
	
	IDPartition() : IDPartition(0,0,-1) {}
	IDPartition(id_t mi, id_t mx, id_t nx) : min(mi), max(mx), nxt(nx) {}

	id_t getNext() {
		return nxt++;
	}

	bool withinRange(id_t num) {
		return num >= min && num <= max;
	}

	bool isBad() {
		return nxt < min || nxt > max;
	}
};
static_assert(std::is_standard_layout<struct IDPartition>::value);
static struct IDPartition LOCAL_PARITION = IDPartition();

struct ConnectionStatus {
	id_t clientId;
	bool isQueue;
	ConnectionStatus() : isQueue(false), clientId(getIdFromPartition(LOCAL_PARITION)) {}

	static id_t getIdFromPartition(const IDPartition& part) {
		return part.min / STD_PARTITION_SIZE;
	}
};
static_assert(std::is_standard_layout<struct ConnectionStatus>::value);

struct Ping {
	long long sent, received;
	
	Ping() : sent(-1), received(-1) {}

	bool isComplete()	{ return sent > 0 && received > 0; }
	long long getTime()	{ return received - sent; }
	void tag() { 
		using namespace std::chrono;
		long long rn = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		if (sent <= 0)
			sent = rn;
		else
			received = rn;
	}
};
static_assert(std::is_standard_layout<struct Ping>::value);

struct IDCorrection {
	ID formerId;
	id_t newId;
};
static_assert(std::is_standard_layout<struct IDCorrection>::value);

struct RequestById {
	ID targetId;
	bool transformOnly;
};
static_assert(std::is_standard_layout<struct RequestById>::value);

//////////////////////////////////////////////////////////////////////////////
//	standarized gameobject messages
//////////////////////////////////////////////////////////////////////////////

//nothing. the objects sort themselves. see Gameobject class.