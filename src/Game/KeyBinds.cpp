#include "KeyBinds.hpp"

using namespace KeyBinds;

bool inline KeyBinds::isEventAllowed(const Event& e)  {
	if (e.is_mouse()) return false;
	if (e.is_character()) return true;

	if (
		e == Event::F11
		|| e == Event::F12
		|| e == Event::Custom
		)
		return false;

	return true;
}

bool KeyBinds::sendEvent(const CONTROL_MODE::_enumerated cm, const Event e) {
	if (!isEventAllowed(e)) return false;

	const auto& ce_ = controls.find(cm);
	if (ce_ == controls.end()) return false;

	const ControlEvents& ce = ce_->second;

	auto ctrl_ = ce.find(EventWrapper(e));

	if (ctrl_ != ce.end()) {
		auto call_ = controlRegistry.find(ctrl_->second);

		if (call_ != controlRegistry.end())
			for (auto& v : call_->second)
				v(e);
	}
}

/* returns true if the funciton was added, false if it already exists
*/
bool KeyBinds::AddControlCall(CONTROLS::_enumerated cs, ControlCall cc) {
	auto ctrlR_ = controlRegistry.find(cs);
	if (ctrlR_ == controlRegistry.end()) {
		//register the command
		controlRegistry.insert({cs, std::vector<ControlCall>{cc}});
		return true;
	}

	for (auto& v : ctrlR_->second)
		// if same funciton dont add it again
		if (v.target<Event(*)>() == cc.target<Event(*)>())
			return false;

	ctrlR_->second.push_back(cc);
	return true;	
}

/*returns true if command was removed, false if it couldnt be found
*/
bool KeyBinds::RemoveControlCall(CONTROLS::_enumerated cs, ControlCall cc) {
	auto ctrl_ = controlRegistry.find(cs);

	if (ctrl_ == controlRegistry.end())//command not linked to anything! (no implimentation???)
		return false;

	for (size_t i = 0; i < ctrl_->second.size(); i++)
		if (ctrl_->second[i].target<Event(*)>() == cc.target<Event(*)>()) {
			if (ctrl_->second.size() == 1)//remove everything about ht ecommand if its the only call left
				controlRegistry.erase(cs);
			else
				ctrl_->second.erase(ctrl_->second.begin() + i); //remove just the specific call;
			return true;
		}

	return false;
}

const KeyBinds::ControlGroup2ContolEvents& KeyBinds::getControls() { return KeyBinds::controls; }
const KeyBinds::Controls2Function& KeyBinds::getControlRegistryMap() { return KeyBinds::controlRegistry; }
const KeyBinds::Control2StringPair& KeyBinds::getInfoMap() { return KeyBinds::infoMap; }

Controls2Function KeyBinds::controlRegistry;

ControlGroup2ContolEvents KeyBinds::controls = {
		{CONTROL_MODE::PLAY, {
				{EventWrapper(Event::Character('n')), CONTROLS::DISPLAY_NEW_WINDOW  },
				{EventWrapper(Event::Character('q')), CONTROLS::MOVEMENT_ALIGN_TO  },
				{EventWrapper(Event::Character('m')), CONTROLS::DISPLAY_TOGGLE_MOVEMENT_OVERLAY  },
				{EventWrapper(Event::Character('s')), CONTROLS::ENGR_INCREASE_PSU  },
				{EventWrapper(Event::Character('x')), CONTROLS::ENGR_DECREASE_PSU  },
				{EventWrapper(Event::Character('a')), CONTROLS::MOVEMENT_INCREASE_DRIVE  },
				{EventWrapper(Event::Character('z')), CONTROLS::MOVEMENT_DECREASE_DRIVE  }
			}}
};
const Control2StringPair KeyBinds::infoMap = {
			{CONTROLS::DISPLAY_NEW_WINDOW, {"open new window", "shows windows avalible to open"}},
			{CONTROLS::DISPLAY_TOGGLE_MOVEMENT_OVERLAY, {"toggle movement overlay", "shows directional vectors of the specified object"}},
			{CONTROLS::ENGR_INCREASE_PSU, {"increase psu output",""}},
			{CONTROLS::ENGR_DECREASE_PSU, {"decrease psu output",""}},
			{CONTROLS::MOVEMENT_ALIGN_TO, {"align to","points ship towards specified position"}},
			{CONTROLS::MOVEMENT_INCREASE_DRIVE, {"increase drive output",""}},
			{CONTROLS::MOVEMENT_DECREASE_DRIVE, {"decrease drive output",""}}
};