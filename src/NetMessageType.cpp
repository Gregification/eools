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

bool Packable::needsMessaged() const
{
	return !_packable_syncTargets.empty();
}

void Packable::packTargets(Message& msg)
{
	//overflow not a problem, results in targets being sent over mutiple 
	//	packets instead of all in 1.
	const uint8_t size = _packable_syncTargets.size();

	auto it = _packable_syncTargets.begin();
	for (int n = 0; n < size; n++) {
		MsgDiffType type = *it;
		++it;

		packMessage(msg, type);
		msg << type;
	}

	msg << size;

	_packable_syncTargets.erase(_packable_syncTargets.begin(), it);
}

void Packable::unpackTargets(Message& msg)
{
	uint8_t n;
	msg >> n;

	for (; n > 0; n--) {
		MsgDiffType type;
		msg >> type;

		unpackMessage(msg, type);
	}
}

bool ID::IsGrid() const
{
	return grid_id == inst_id;
}

Instance_Id IDPartition::getNext()
{
	return nxt++;
}

bool IDPartition::withinRange(Instance_Id num) const
{
	return num >= min && num <= max;
}

bool IDPartition::IsBad() const
{
	return nxt < min || nxt > max;
}

void OTHERMSG::ChatMsg::packMessage(Message& m, MsgDiffType)
{
	size_t len = std::min(str.length(), MAX_LEN);

	for (int i = len - 1; i > 0; i--)
		m << str[i];
	m << len;
}

void OTHERMSG::ChatMsg::unpackMessage(Message& m, MsgDiffType)
{
	size_t len;
	m >> len;

	str.resize(len);

	for (int i = 0; i < len; i++)
		m >> str[i]; //lots of potential for screw up
}
