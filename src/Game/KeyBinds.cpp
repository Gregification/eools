#include "KeyBinds.hpp"

using namespace KeyBinds;

bool inline KeyBinds::isEventAllowed(const Event& e)  {
	if (e.is_mouse()) return false;
	if (e.is_character()) return true;

	if (
		e == Event::Character('L')	//ftxui reserved
		|| e == Event::F11
		|| e == Event::F12
		|| e == Event::Custom
		)
		return false;

	return true;
}

/*returns same as KeyBinds::isEventAllowed
*/
bool KeyBinds::sendEvent(Event e) {
	if (!isEventAllowed(e)) return false;

	auto ew = EventWrapper(std::move(e));

	if (key_to_controls.contains(ew)) {
		for (const auto& v : key_to_controls[ew])
			for (const auto& j : control_to_subs[v])
				j(e);

		return true;
	} 
	
	return false;
}

bool KeyBinds::SubToCtrlEvnt(CONTROL_EVENT::_enumerated cs, ControlCall cc) {
	auto& subs = control_to_subs[cs];

	subs.push_back(cc);

	return subs.size() == 1;	
}

size_t KeyBinds::ClearSubsOfCtrlEvnt(CONTROL_EVENT::_enumerated ce) {
	auto &arr = control_to_subs[ce];
	size_t ret = arr.size();;
	arr.clear();
	return ret;
}

bool KeyBinds::SubCtrlEvntToKeyEvnt(Event e, CONTROL_EVENT::_enumerated ce){
	auto& arr = key_to_controls[EventWrapper(std::move(e))];

	//if already exists
	if (arr.size() != 0 && std::find(arr.begin(), arr.end(), ce) != arr.end())
		return false;
	else
		arr.push_back(ce);

	return true;
}

bool KeyBinds::UnsubCtrlEvntToKeyEvnt(Event e, CONTROL_EVENT::_enumerated ce){
	auto& arr = key_to_controls[EventWrapper(std::move(e))];

	//if already exists
	auto it = std::find(arr.begin(), arr.end(), ce);

	if (it == arr.end())
		return false;
	
	arr.erase(it);
	return true;
}

size_t KeyBinds::ClearCtrlEvntsOfKeyEvnt(Event e) {
	auto& arr = key_to_controls[EventWrapper(e)];
	size_t ret = arr.size();;
	arr.clear();
	return ret;
}

const KeyBinds::KeyEvent2ControlEvent& KeyBinds::getKeyEvnt2CtrlEvntMap()	{ return KeyBinds::key_to_controls; }
const KeyBinds::Controls2Functions& KeyBinds::getCtrlEvnt2SubsMap()			{ return KeyBinds::control_to_subs; }
const KeyBinds::Control2StringPairs& KeyBinds::getInfoMap() { return KeyBinds::infoMap; }

Controls2Functions KeyBinds::control_to_subs;

KeyEvent2ControlEvent KeyBinds::key_to_controls = {
	{EventWrapper(Event::Character('n')), {CONTROL_EVENT::DISPLAY_NEW_WINDOW}  },
	{EventWrapper(Event::Character('q')), {CONTROL_EVENT::MOVEMENT_ALIGN_TO}  },
	{EventWrapper(Event::Character('m')), {CONTROL_EVENT::DISPLAY_TOGGLE_MOVEMENT_OVERLAY}  },
	{EventWrapper(Event::Character('s')), {CONTROL_EVENT::ENGR_INCREASE_PSU}  },
	{EventWrapper(Event::Character('x')), {CONTROL_EVENT::ENGR_DECREASE_PSU}  },
	{EventWrapper(Event::Character('a')), {CONTROL_EVENT::MOVEMENT_INCREASE_DRIVE}  },
	{EventWrapper(Event::Character('z')), {CONTROL_EVENT::MOVEMENT_DECREASE_DRIVE}  }
};
const Control2StringPairs KeyBinds::infoMap = {
			{CONTROL_EVENT::DISPLAY_NEW_WINDOW, {"open new window", "shows windows avalible to open"}},
			{CONTROL_EVENT::DISPLAY_TOGGLE_MOVEMENT_OVERLAY, {"toggle movement overlay", "shows directional vectors of the specified object"}},
			{CONTROL_EVENT::ENGR_INCREASE_PSU, {"increase psu output",""}},
			{CONTROL_EVENT::ENGR_DECREASE_PSU, {"decrease psu output",""}},
			{CONTROL_EVENT::MOVEMENT_ALIGN_TO, {"align to","points ship towards specified position"}},
			{CONTROL_EVENT::MOVEMENT_INCREASE_DRIVE, {"increase drive output",""}},
			{CONTROL_EVENT::MOVEMENT_DECREASE_DRIVE, {"decrease drive output",""}}
};