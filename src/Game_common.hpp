#pragma once

#include <cstdint>

#define SERVER_PORT 60000

#define STD_PARTITION_SIZE 10'000'000 //100 mill => ~200 unique parts

//UPDATE: this is not it
//GameObject time units. i got tired of discovering naming conflicts.
//32 bits gets accuracy to 7.1 weeks
//union go_time {
//	uint32_t b32;
//	uint64_t b64;
//
//    //void setBy32(const long long& ll) {
//	//	this.b32 = static_cast<uint32_t>(ll & 0xFF'FF'FF'FF);
//	//}
//
//	uint32_t getUint32() {
//		return b32;
//	}
//};

//should be -1. GameObjectFactory uses BAD+1 as index.
//since it wasent working with hash maps it uses a vector that requires it starts at 0 ... spaghettie code
#define BAD_CLASS_ID -1
using class_id		= uint8_t;	//class id

//should be 0. server partition starts ar BAD_ID+1
#define BAD_ID 0
using inst_id	= uint64_t;	//instance id