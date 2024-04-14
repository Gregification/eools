#pragma once

#include <cstdint>

#define SERVER_PORT 60000

#define STD_PARTITION_SIZE 100'000'000 //100 mill => ~20 unique parts
#define BAD_ID 0 //should be 0. server partition starts ar BAD_ID+1

using cid_t		= uint8_t;	//class id
using id_t		= uint64_t;	//instance id