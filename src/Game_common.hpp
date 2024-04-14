#pragma once

#include <cstdint>

#define SERVER_PORT 60000

#define STD_PARTITION_SIZE 100'000'000 //100 mill => ~20 unique parts
#define BAD_ID 0 //should be 0. server partition starts ar BAD_ID+1

using id_t = int64_t;