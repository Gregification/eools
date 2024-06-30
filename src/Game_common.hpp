#pragma once

#include <cstdint>

#define SERVER_PORT 60000
#define STD_PARTITION_SIZE 10'000'000 //100 mill => ~200 unique parts

//since it wasent working with hash maps it uses a vector that requires it starts at 0 ... spaghettie code
#define BAD_CLASS_ID -1
typedef	uint8_t Class_Id;

//should be 0. server partition starts ar BAD_ID+1
#define BAD_ID 0
typedef uint64_t Instance_Id;	//instance id

typedef float Unit;

/******************************************************************************
* game stuff
******************************************************************************/

//units : multiply the num by these ratios to get their respective values
//see https://en.wikipedia.org/wiki/Standard_gravitational_parameter
#define KM 0.000'001
#define KT 0.001
#define KJ 0.000'000'001
#define LS KM * 299'792.458
#define AU KM * 14'959'787.069'1
#define GRAV_CONST 6.67430e-11
