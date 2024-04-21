#pragma once

#include <cstdint>

#define SERVER_PORT 60000

#define STD_PARTITION_SIZE 100'000'000 //100 mill => ~20 unique parts

//GameObject time units. pardon the nameing, i got tired of discovering naming conflicts.
//seems like to get millisecond accuracy to the week requires 32 (31.78) bits
//though using 32 bits exactly suggests that the server will massively desync every ~1+ week?
//honestly just use 64 bits. stuffs basically free
struct go_time {
	uint32_t value = 0;

    go_time() : go_time(0) {}
    go_time(uint32_t val) : value(val) {}

    //the only special opeartor
    //copies the last 32 bits
	go_time& operator=(const long long& ll) {
		value = static_cast<uint32_t>(ll & 0xFF'FF'FF'FF);//copy the last 32 bits
		return *this;
	}

    //i have no idea how else to get go_time to behave like a normal number
    //normal mathmatical, compairson, and bitise operators
    go_time operator+   (const go_time& other)  const { return go_time(value + other.value); }
    go_time operator-   (const go_time& other)  const { return go_time(value - other.value); }
    go_time operator*   (const go_time& other)  const { return go_time(value * other.value); }
    go_time operator/   (const go_time& other)  const { return go_time(value / other.value); }
    go_time operator%   (const go_time& other)  const { return go_time(value % other.value); }
    go_time operator&   (const go_time& other)  const { return go_time(value & other.value); }
    go_time operator|   (const go_time& other)  const { return go_time(value | other.value); }
    go_time operator^   (const go_time& other)  const { return go_time(value ^ other.value); }
    go_time operator<<  (const int      shift)  const { return go_time(value << shift); }
    go_time operator>>  (const int      shift)  const { return go_time(value >> shift); }
    bool    operator==  (const go_time& other)  const { return value == other.value; }
    bool    operator!=  (const go_time& other)  const { return value != other.value; }
    bool    operator<   (const go_time& other)  const { return value < other.value; }
    bool    operator>   (const go_time& other)  const { return value > other.value; }
    bool    operator<=  (const go_time& other)  const { return value <= other.value; }
    bool    operator>=  (const go_time& other)  const { return value >= other.value; }
};
static_assert(std::is_standard_layout<go_time>::value);

//should be -1. GameObjectFactory uses BAD+1 as index.
//since it wasent working with hash maps it uses a vector that requires it starts at 0 ... spaghettie code
#define BAD_CLASS_ID -1
using class_id		= uint8_t;	//class id

//should be 0. server partition starts ar BAD_ID+1
#define BAD_ID 0
using inst_id	= uint64_t;	//instance id