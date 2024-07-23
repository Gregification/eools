#include "Navigation.hpp"

#include "../GameObjects/Ship.hpp"

using namespace Navigation;

/***********************************************************************************
* setup 
*  - semi boiler plate
***********************************************************************************/

std::unordered_map<TRAVEL_STATE::_enumerated, std::function<std::unique_ptr<NavBase>()>> 
Navigation::stateToNavigator{
	{TRAVEL_STATE::NONE,				[] { return  nullptr; }},
	{TRAVEL_STATE::ALIGN,				[] { return  std::make_unique<ALIGN>(); }},
	{TRAVEL_STATE::ALIGN_TO,			[] { return  std::make_unique<ALIGN_TO>(); }},
	{TRAVEL_STATE::APPROACH,			[] { return  std::make_unique<APPROACH>(); }},
	{TRAVEL_STATE::MAINTAIN_DISTANCE,	[] { return  std::make_unique<MAINTAIN_DISTANCE>(); }},
	{TRAVEL_STATE::ORBIT,				[] { return  std::make_unique<ORBIT>(); }},
};

//its either this or give up split screen
constexpr TRAVEL_STATE::_enumerated ALIGN::getTravelState() { 
				return TRAVEL_STATE::ALIGN; };
constexpr TRAVEL_STATE::_enumerated ALIGN_TO::getTravelState() { 
				return TRAVEL_STATE::ALIGN_TO; };
constexpr TRAVEL_STATE::_enumerated APPROACH::getTravelState() { 
				return TRAVEL_STATE::APPROACH; };
constexpr TRAVEL_STATE::_enumerated MAINTAIN_DISTANCE::getTravelState() { 
				return TRAVEL_STATE::MAINTAIN_DISTANCE; };
constexpr TRAVEL_STATE::_enumerated ORBIT::getTravelState() { 
				return TRAVEL_STATE::ORBIT; };


void NavInfo::packMessage(Message& msg, MsgDiffType mdt) {
	if (navPattern) {
		//cast should be unecessary but i dont trust i'll forget to update this later
		msg << (TRAVEL_STATE::_enumerated)navPattern->getTravelState();

		navPattern->packMessage(msg, mdt);
	} else {
		msg << TRAVEL_STATE::_enumerated::NONE;
	}
}
void NavInfo::unpackMessage(Message& msg, MsgDiffType mdt) {
	TRAVEL_STATE::_enumerated ts;
	msg >> ts;

	if (ts == TRAVEL_STATE::_enumerated::NONE) 
		return;

	//tests should have ensured every state has associated pattern
	navPattern = stateToNavigator[ts]();

	navPattern->unpackMessage(msg, mdt);
}

/***********************************************************************************
* actual nav pattern implimentations
***********************************************************************************/

void ALIGN::packMessage(Message& msg, MsgDiffType) {}
void ALIGN::unpackMessage(Message& msg, MsgDiffType) {}
void ALIGN::refresh() {}
void ALIGN::nav_update(float) {}

void ALIGN_TO::packMessage(Message& msg, MsgDiffType) {}
void ALIGN_TO::unpackMessage(Message& msg, MsgDiffType) {}
void ALIGN_TO::refresh() {}
void ALIGN_TO::nav_update(float) {}

void APPROACH::packMessage(Message& msg, MsgDiffType) {}
void APPROACH::unpackMessage(Message& msg, MsgDiffType) {}
void APPROACH::refresh() {}
void APPROACH::nav_update(float) {}

void MAINTAIN_DISTANCE::packMessage(Message& msg, MsgDiffType) {}
void MAINTAIN_DISTANCE::unpackMessage(Message& msg, MsgDiffType) {}
void MAINTAIN_DISTANCE::refresh() {}
void MAINTAIN_DISTANCE::nav_update(float) {}

void ORBIT::packMessage(Message& msg, MsgDiffType) {}
void ORBIT::unpackMessage(Message& msg, MsgDiffType) {}
void ORBIT::refresh() {}
void ORBIT::nav_update(float) {}
