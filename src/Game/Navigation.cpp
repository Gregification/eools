#include "Navigation.hpp"

#include "../GameObjects/Ship.hpp"

#include "Input Controllers/nav/ICNavAlign.hpp"

using namespace Navigation;

/***********************************************************************************
* setup 
*  - semi boiler plate
*  - fiddle around with this if your trying to do something like have a pattern 
*		be interperted as another for some reason
***********************************************************************************/

std::unordered_map<TRAVEL_STATE::_enumerated, std::function<InputControl<NavBase>()>>
Navigation::stateToNavigatorIC{
	{TRAVEL_STATE::NONE,				[] { return InputControl<NavBase>{ICNavAlign()}; }},
	{TRAVEL_STATE::ALIGN,				[] { return InputControl<NavBase>{ICNavAlign()}; }},
	{TRAVEL_STATE::ALIGN_TO,			[] { return InputControl<NavBase>{ICNavAlign()}; }},
	{TRAVEL_STATE::APPROACH,			[] { return InputControl<NavBase>{ICNavAlign()}; }},
	{TRAVEL_STATE::ORBIT,				[] { return InputControl<NavBase>{ICNavAlign()}; }},
	{TRAVEL_STATE::MAINTAIN_DISTANCE,	[] { return InputControl<NavBase>{ICNavAlign()}; }},
};

std::unordered_map<TRAVEL_STATE::_enumerated, std::function<std::unique_ptr<NavBase>()>> 
Navigation::stateToNavigator{
	{TRAVEL_STATE::NONE,				[] { return nullptr; }},//special case
	{TRAVEL_STATE::ALIGN,				[] { return std::make_unique<ALIGN>(); }},
	{TRAVEL_STATE::ALIGN_TO,			[] { return std::make_unique<ALIGN_TO>(); }},
	{TRAVEL_STATE::APPROACH,			[] { return std::make_unique<APPROACH>(); }},
	{TRAVEL_STATE::MAINTAIN_DISTANCE,	[] { return std::make_unique<MAINTAIN_DISTANCE>(); }},
	{TRAVEL_STATE::ORBIT,				[] { return std::make_unique<ORBIT>(); }},
};

//its either this or give up split screen
TRAVEL_STATE::_enumerated ALIGN::getTravelState() const  { 
				return TRAVEL_STATE::ALIGN; };
TRAVEL_STATE::_enumerated ALIGN_TO::getTravelState() const { 
				return TRAVEL_STATE::ALIGN_TO; };
TRAVEL_STATE::_enumerated APPROACH::getTravelState() const { 
				return TRAVEL_STATE::APPROACH; };
TRAVEL_STATE::_enumerated MAINTAIN_DISTANCE::getTravelState() const { 
				return TRAVEL_STATE::MAINTAIN_DISTANCE; };
TRAVEL_STATE::_enumerated ORBIT::getTravelState() const { 
				return TRAVEL_STATE::ORBIT; };


void Navigation::NavInfo::setNavPattern(NavBase nnp)
{
	navPattern.release();
	navPattern = std::make_unique<NavBase>(std::move(nnp));
}

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
*  - more boiler plate!!! keep reading for explinations
*  - the message un/packing is something worth considering since these structs 
*		are intended to cache values and may hold non standard layout objects
*		which means it cant just be shuffled into a 'Message' using the shift 
*		operator. DO NOT USE THE SHIFT OP FOR MESSAGES. these are not standard
*		layout structs because they have vitrual stuff, therefore shift operator
*		will be putting out garbage! gigo.
***********************************************************************************/

void ALIGN::packMessage(Message& msg, MsgDiffType) {
	msg << targetRot;
}
void ALIGN::unpackMessage(Message& msg, MsgDiffType) {
	msg >> targetRot;
}
void ALIGN::reset() {

}
void ALIGN::nav_update(float dt) {
	if (auto s = ship.lock()) {
		s->transform.rotation.rotateBy(targetRot - s->transform.rotation.getRotation());
	}
}

void ALIGN_TO::packMessage(Message& msg, MsgDiffType) { }
void ALIGN_TO::unpackMessage(Message& msg, MsgDiffType) { }
void ALIGN_TO::reset() {}
void ALIGN_TO::nav_update(float dt) {}

void APPROACH::packMessage(Message& msg, MsgDiffType) { }
void APPROACH::unpackMessage(Message& msg, MsgDiffType) { }
void APPROACH::reset() {}
void APPROACH::nav_update(float dt) {}

void MAINTAIN_DISTANCE::packMessage(Message& msg, MsgDiffType) { }
void MAINTAIN_DISTANCE::unpackMessage(Message& msg, MsgDiffType) { }
void MAINTAIN_DISTANCE::reset() {}
void MAINTAIN_DISTANCE::nav_update(float dt) {}

void ORBIT::packMessage(Message& msg, MsgDiffType) {  }
void ORBIT::unpackMessage(Message& msg, MsgDiffType) { }
void ORBIT::reset() {}
void ORBIT::nav_update(float dt) {}
