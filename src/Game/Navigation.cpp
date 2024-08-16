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

std::unordered_map<TRAVEL_STATE::_enumerated, std::function<InputController()>>
Navigation::stateToNavigatorIC{
	{TRAVEL_STATE::NONE,				[] { return std::make_shared<ICNavAlign>(); }},
	{TRAVEL_STATE::ALIGN,				[] { return std::make_shared<ICNavAlign>(); }},
	{TRAVEL_STATE::ALIGN_TO,			[] { return std::make_shared<ICNavAlign>(); }},
	{TRAVEL_STATE::APPROACH,			[] { return std::make_shared<ICNavAlign>(); }},
	{TRAVEL_STATE::ORBIT,				[] { return std::make_shared<ICNavAlign>(); }},
	{TRAVEL_STATE::MAINTAIN_DISTANCE,	[] { return std::make_shared<ICNavAlign>(); }},
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


void NavInfo::setNavPattern(std::unique_ptr<NavBase> nnp)
{
	navPattern.release();
	navPattern = std::move(nnp);
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

	//not bothering with a factory since this is limited and very specific
	// also need some flexability in instantiating that a factory wont allow

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
void ALIGN::reset(Ship& s) {

}
void ALIGN::nav_update(float dt, Ship& ship) {
	//	- unrelated : https://stackoverflow.com/questions/53990369/is-modulo-a-verb-if-so-how-is-it-conjugated
	
	const float& v = ship.transform.angularVelocity;
	const float a = ship.driveTrain.u_rotAccele * rate;

	const float d = RotationHandler::RotDiff(
		ship.transform.rotation.getRotation(),
		targetRot
	);

	const float tv = v + dt * std::min(a, std::max(-a, d * a - v));

	if (std::abs(tv) < 0.00001)
		ship.transform.angularVelocity = 0;
	else
		ship.transform.angularVelocity = tv;
}

void ALIGN_TO::packMessage(Message& msg, MsgDiffType) { }
void ALIGN_TO::unpackMessage(Message& msg, MsgDiffType) { }
void ALIGN_TO::reset(Ship&) {}
void ALIGN_TO::nav_update(float dt, Ship& s) {}

void APPROACH::packMessage(Message& msg, MsgDiffType) { }
void APPROACH::unpackMessage(Message& msg, MsgDiffType) { }
void APPROACH::reset(Ship&) {}
void APPROACH::nav_update(float dt, Ship& s) {}

void MAINTAIN_DISTANCE::packMessage(Message& msg, MsgDiffType) { }
void MAINTAIN_DISTANCE::unpackMessage(Message& msg, MsgDiffType) { }
void MAINTAIN_DISTANCE::reset(Ship&) {}
void MAINTAIN_DISTANCE::nav_update(float dt, Ship& s) {}

void ORBIT::packMessage(Message& msg, MsgDiffType) {  }
void ORBIT::unpackMessage(Message& msg, MsgDiffType) { }
void ORBIT::reset(Ship&) {}
void ORBIT::nav_update(float dt, Ship& s) {}

void Navigation::NavBase::reset(Ship&)
{
}

void Navigation::NavBase::nav_update(float, Ship&)
{
}

TRAVEL_STATE::_enumerated Navigation::NavBase::getTravelState() const
{
	return TRAVEL_STATE::NONE;
}
