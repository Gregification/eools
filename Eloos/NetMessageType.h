#pragma once

#include <cstdint>

enum class NetMsgType : uint32_t {
	ServerMessage,
	Ping
};