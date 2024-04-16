#pragma once

#include <cstdint>

#define SERVER_PORT 60000

#define STD_PARTITION_SIZE 100'000'000 //100 mill => ~20 unique parts

//should be 0. server partition starts ar BAD_ID+1
#define BAD_ID 0

//should be -1. GameObjectFactory uses BAD+1 as index.
//since it wasent working with hash maps it uses a vector that requires it starts at 0 ... spaghettie code
#define BAD_CLASS_ID -1

using cid_t		= uint8_t;	//class id
using id_t		= uint64_t;	//instance id