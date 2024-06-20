#pragma once

#include <ftxui/component/event.hpp>
#include <ftxui/component/mouse.hpp>

using ftxui::Event;

//manages keybinds for Client. (keyboard only, mouse buttons and movement not included!)
//	note that this only applys to the primary component of each renderer! events can be intercepted before hand!
//	key combos not supported!!! (aside form capital/lowercase letters)
//	customevents not accounted for!!! (will tell you)

class KeyBinds {
public:
	KeyBinds() = delete;



	bool inline isEventAllowed(Event& const e) const {
		if (e.is_character()) return true;
		if (e.is_mouse()) return false;

		if (
			e == Event::F11
			|| e == Event::F12
			|| e == Event::Custom
			)
			return false;

		return true;
	}
	
	struct CONTROLLS_PLAY {
		static const CONTROLLS_PLAY NEW_WINDOW;
		static const CONTROLLS_PLAY ALIGN_TO;
		static const CONTROLLS_PLAY TOGGLE_MOVEMENT_OVERLAY;
		static const CONTROLLS_PLAY MOVEMENT_INCREASE_THROTTLE;
		static const CONTROLLS_PLAY MOVEMENT_DECREASE_THROTTLE;

		const std::string displayName;

		private:
			CONTROLLS_PLAY(const std::string& name) : displayName(name) {}
	};

	bool sendEvent(Event e) {

	}
};