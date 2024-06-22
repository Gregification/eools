#pragma once

#include <unordered_map>
#include <functional>

#include <ftxui/component/event.hpp>
#include "../better-enums/enum.h"

//manages keybinds for Client. (keyboard only, mouse buttons and movement not included!)
// THIS IS SPECIFICALLY MADE FOR THE CLIENT "PLAY" MODE. it was getting messy otherwise
//	note that this only applys to the primary component of each renderer! events can be intercepted before hand!
//	key combos not supported!!! (aside form capital/lowercase letters)
//	customevents not accounted for!!! (indicated in return tell you)

namespace KeyBinds { //class becuase its easier to read
	using ftxui::Event;

	//c++ compiler needs exploded. if this is a struct it cannot have static values or else unordered maps cannot use it as a key.
	//all controls go here, i give up trying ot make a way for these to be mode-seperated. just visually seperate them and remember what goes where.
	BETTER_ENUM(CONTROL_EVENT, int,
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

	typedef std::function<void(Event)>
		ControlCall;
	typedef std::unordered_map<EventWrapper, std::vector<CONTROL_EVENT::_enumerated>, EventWrapperHasher>		
		KeyEvent2ControlEvent;
	typedef std::unordered_map<CONTROL_EVENT::_enumerated, std::vector<ControlCall>>
		Controls2Functions;
	typedef std::unordered_map<CONTROL_EVENT::_enumerated, const std::pair< const std::string, const std::string>> 
		Control2StringPairs;

	extern bool inline isEventAllowed(const Event&);
	extern bool sendEvent(Event);

	/* returns true if it's the only funciton registered*/
	extern bool SubToCtrlEvnt(CONTROL_EVENT::_enumerated, ControlCall);
	/*removes everything, returns # of items removed*/
	extern size_t ClearSubsOfCtrlEvnt(CONTROL_EVENT::_enumerated);

	/*returns true if event was added, false if already exists*/
	extern bool SubCtrlEvntToKeyEvnt(Event, CONTROL_EVENT::_enumerated);
	/*returns true if event was removed, false if couldnt be found*/
	extern bool UnsubCtrlEvntToKeyEvnt(Event, CONTROL_EVENT::_enumerated);
	/*removes everything, returns # of items removed*/
	extern size_t ClearCtrlEvntsOfKeyEvnt(Event);

	extern const KeyEvent2ControlEvent& getKeyEvnt2CtrlEvntMap();
	extern const Controls2Functions& getCtrlEvnt2SubsMap();
	extern const Control2StringPairs& getInfoMap();

	extern const Control2StringPairs	infoMap;
	extern Controls2Functions			control_to_subs;
	extern KeyEvent2ControlEvent		key_to_controls;
};
