#pragma once

#include <cstdint>

#define SERVER_PORT 60000

enum class NetMsgType : uint32_t {
	ServerMessage,
	Ping
};