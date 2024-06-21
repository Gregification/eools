#pragma once

#include <unordered_map>
#include <functional>

#include <ftxui/component/event.hpp>
#include "../better-enums/enum.h"

//manages keybinds for Client. (keyboard only, mouse buttons and movement not included!)
//	note that this only applys to the primary component of each renderer! events can be intercepted before hand!
//	key combos not supported!!! (aside form capital/lowercase letters)
//	customevents not accounted for!!! (indicated in return tell you)

namespace KeyBinds { //class becuase its easier to read
	using ftxui::Event;

	BETTER_ENUM(CONTROL_MODE, int,
		PLAY,
		MAP,
		INVENTORY,
		RESEARCH
	)
	
	//c++ compiler needs exploded. if this is a struct it cannot have static values or else unordered maps cannot use it as a key.
	//all controls go here, i give up trying ot make a way for these to be mode-seperated. just visually seperate them and remember what goes where.
	BETTER_ENUM(CONTROLS, int,
		DISPLAY_NEW_WINDOW,
		DISPLAY_TOGGLE_MOVEMENT_OVERLAY,
		MOVEMENT_ALIGN_TO,
		ENGR_INCREASE_PSU,
		ENGR_DECREASE_PSU,
		MOVEMENT_INCREASE_DRIVE,
		MOVEMENT_DECREASE_DRIVE
	);

	struct EventWrapper {	//makes ftxui::Event useable as a std::map key
		explicit EventWrapper(ftxui::Event evnt) : event_(evnt) {}

		const Event& event() const { return event_; }
		
		auto operator<=>(const EventWrapper&) const = default;
	private:
		Event event_;

	};
	//templaet<> std::hash dosent want to work. idk
	struct EventWrapperHasher {
		size_t operator()(const EventWrapper& w) const {
			return std::hash<std::string>()(w.event().input());
		}
	};

	typedef std::function<void(Event)> ControlCall;
	typedef std::unordered_map<EventWrapper, CONTROLS::_enumerated, EventWrapperHasher> ControlEvents;

	typedef std::unordered_map<CONTROLS::_enumerated, std::vector<ControlCall>> Controls2Function;
	typedef std::unordered_map<CONTROL_MODE::_enumerated, ControlEvents> ControlGroup2ContolEvents;
	typedef std::unordered_map<CONTROLS::_enumerated, std::pair<std::string, std::string>> Control2StringPair;

	extern Controls2Function controlRegistry;
	
	extern bool inline isEventAllowed(const Event&);
	extern bool sendEvent(const CONTROL_MODE::_enumerated, const Event);

	extern bool AddControlCall(CONTROLS::_enumerated, ControlCall);
	extern bool RemoveControlCall( CONTROLS::_enumerated, ControlCall);

	extern const ControlGroup2ContolEvents& getControls();
	extern const Controls2Function& getControlRegistryMap();
	extern const Control2StringPair& getInfoMap(const CONTROLS&);

	extern const Control2StringPair infoMap;
	extern ControlGroup2ContolEvents controls;

};
