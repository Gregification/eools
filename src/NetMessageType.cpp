#pragma once

#include "NetMessageType.hpp"

IDPartition IDPartition::LOCAL_PARITION{};

time_t Ping::tag() {
	using namespace std::chrono;
	time_t rn = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	if (sent <= 0)
		sent = rn;
	else
		received = rn;

	return getTime();
}
