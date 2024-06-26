#include "Events.hpp"

using namespace Events;

size_t KeyBinds::sendKey(ftxui::Event e) {		
	size_t ret = 0;
	auto& arr = KeyMap[e];
	for(auto& a : arr)
		observer.invokeEvent(a);
	return ret;
}

bool KeyBinds::registerCtrlEvent(ftxui::Event e, CONTROL_EVENT c) {
	auto& arr = KeyMap[std::move(e)];

	if (std::find(arr.begin(), arr.end(), c) == arr.end()) {
		arr.push_back(std::move(c));
		return true;
	}

	return false;
}

void KeyBinds::unregisterCtrlEvent(ftxui::Event e, CONTROL_EVENT c) {
	auto& arr = KeyMap[std::move(e)];

	arr.erase(std::remove(arr.begin(), arr.end(), c), arr.end());
}


/*****************************************************************************************************************
* default keybinds
*****************************************************************************************************************/

KeyBinds::Key2ControlEvents KeyBinds::KeyMap = {
	{ftxui::Event::Character('n'),	{KeyBinds::CONTROL_EVENT::DISPLAY_NEW_WINDOW}  },
	{ftxui::Event::Character('m'),	{KeyBinds::CONTROL_EVENT::DISPLAY_TOGGLE_MOVEMENT_OVERLAY}  },
	{ftxui::Event::Character('t'),	{KeyBinds::CONTROL_EVENT::DISPLAY_REMOVE_WINDOW}},
	{ftxui::Event::Character('s'),	{KeyBinds::CONTROL_EVENT::ENGR_INCREASE_PSU}  },
	{ftxui::Event::Character('x'),	{KeyBinds::CONTROL_EVENT::ENGR_DECREASE_PSU}  },
	{ftxui::Event::Character('q'),	{KeyBinds::CONTROL_EVENT::MOVEMENT_ALIGN_TO}  },
	{ftxui::Event::Character('a'),	{KeyBinds::CONTROL_EVENT::MOVEMENT_INCREASE_DRIVE}  },
	{ftxui::Event::Character('z'),	{KeyBinds::CONTROL_EVENT::MOVEMENT_DECREASE_DRIVE}  },
};

/*****************************************************************************************************************
* default name & description entries
*****************************************************************************************************************/

Observer<KeyBinds::CONTROL_EVENT> KeyBinds::observer = {{
	{CONTROL_EVENT::DISPLAY_NEW_WINDOW, {"open new window", "shows windows avalible to open"}},
	{CONTROL_EVENT::DISPLAY_TOGGLE_MOVEMENT_OVERLAY, {"toggle movement overlay", "shows directional vectors of the specified object"}},
	{CONTROL_EVENT::DISPLAY_REMOVE_WINDOW, {"remove selected window", "removes the latest selected window"}},
	{CONTROL_EVENT::ENGR_INCREASE_PSU, {"increase psu output",""}},
	{CONTROL_EVENT::ENGR_DECREASE_PSU, {"decrease psu output",""}},
	{CONTROL_EVENT::MOVEMENT_ALIGN_TO, {"align to","points ship towards specified position"}},
	{CONTROL_EVENT::MOVEMENT_INCREASE_DRIVE, {"increase drive output",""}},
	{CONTROL_EVENT::MOVEMENT_DECREASE_DRIVE, {"decrease drive output",""}},
}};

Observer<ClientEvent::CLIENT_EVENT> ClientEvent::observer = { {
	{CLIENT_EVENT::EVENT_MESSAGE, {"messages to the user", "shows windows avalible to open"}},
} };

Observer<Network::NETWORK_EVENT> Network::observer = { {
	{Network::NETWORK_EVENT::SEND_MESSAGE, {"passes message along connection", "implimentation dependent on client or server"}},
} };
