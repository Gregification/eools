#include "Events.hpp"

using namespace Events;

bool KeyBinds::sendKey(ftxui::Event e) {
	const auto& arr = KeyMap[e];

	for(auto& a : arr)
		observer.invokeEvent(a);

	return arr.size() != 0;
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
* - unable to refrence special ftxui::Event's directly because they are assigned
*		at runtime after this namespace. therefore th 'input_' will be empty.
*		solution is to hardcode the value.
*****************************************************************************************************************/

KeyBinds::Key2ControlEvents KeyBinds::KeyMap = {
	{ftxui::Event::Character('o'),		{KeyBinds::CONTROL_EVENT::DEBUG_btn}  },
	{ftxui::Event::Special("\x1B"),		{KeyBinds::CONTROL_EVENT::ESCAPE}  },
	{ftxui::Event::Character('n'),		{KeyBinds::CONTROL_EVENT::DISPLAY_NEW_WINDOW}  },
	{ftxui::Event::Character('m'),		{KeyBinds::CONTROL_EVENT::DISPLAY_TOGGLE_MOVEMENT_OVERLAY}  },
	{ftxui::Event::Special("\x1B[3~"),	{KeyBinds::CONTROL_EVENT::DISPLAY_REMOVE_WINDOW}},
	{ftxui::Event::Character('x'),		{KeyBinds::CONTROL_EVENT::SELECT_SELF_SHIP}  },
};

/*****************************************************************************************************************
* default name & description entries
*****************************************************************************************************************/

Observer<KeyBinds::CONTROL_EVENT> KeyBinds::observer = {{
	{CONTROL_EVENT::DEBUG_btn, {"debug button", "a button for debugging"}},
	{CONTROL_EVENT::ESCAPE, {"excape", "cancels pending actions and what ever"}},
	{CONTROL_EVENT::DISPLAY_NEW_WINDOW, {"open new window", "shows windows avalible to open"}},
	{CONTROL_EVENT::DISPLAY_TOGGLE_MOVEMENT_OVERLAY, {"toggle movement overlay", "shows directional vectors of the specified object"}},
	{CONTROL_EVENT::DISPLAY_REMOVE_WINDOW, {"remove selected window", "removes the latest selected window"}},
	{CONTROL_EVENT::SELECT_SELF_SHIP, {"select players ship", "selects the orgional ship"}},
}};

//this is somewhat pointless since the client wont care and this is for ui
//keep it around for organization reasons
Observer<ClientEvent::CLIENT_EVENT> ClientEvent::observer = { {
	{CLIENT_EVENT::EVENT_MESSAGE, {"general events", "forwards events that the user may find usefull"}},
	{CLIENT_EVENT::ON_SHIP_OPERABLE_SHIP_FOCOUS, {"on operable ship select", "triggered when a operable ship is selected"}},
	{CLIENT_EVENT::ON_WINDOW_FOCUS, {"on window focus", ""}},
	{CLIENT_EVENT::ON_WINDOW_UNFOCUS, {"on window unfocus", ""}},
	{CLIENT_EVENT::ADD_TO_WINDOW_CONTAINER, {"open window", ""}},
	{CLIENT_EVENT::ON_GAMEOBJECT_SELECT, {"on gameobject select", ""}},
	{CLIENT_EVENT::ADD_RESOLVEABLE_RESPONDER, {"new resolver to handler", ""}},
} };

Observer<Network::NETWORK_EVENT> Network::observer = { {
	{Network::NETWORK_EVENT::SEND_MESSAGE, {"passes message along connection", "implimentation dependent on client or server"}},
} };
