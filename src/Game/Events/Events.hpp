#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include <ftxui/component/event.hpp>
#include "../../better-enums/enum.h"

namespace Events {
	typedef std::function<void()> 
		Listener;
	template<typename EVENT> //should be enum. DO NOT USE BETTER_ENUM
	class Observer {
	private:
		typedef std::unordered_map<EVENT, std::vector<Listener>>
			Event2Subs;
		typedef std::unordered_map<EVENT, const std::pair< const std::string, const std::string>>
			Event2StringPair;

		Event2Subs event_to_subscribers;

	public:
		Observer(Event2StringPair e) : event_name_and_description(e) {}
		const Event2StringPair event_name_and_description;

		/*triggers a event if it's allowed. returns same as KeyBinds::isEventAllowed*/
		bool invokeEvent(EVENT e) {
			if (!isEventAllowed(e)) return false;
			forceInvokeEvent(std::move(e));
			return true;
		}
		/*guarentted to trigger event*/
		void forceInvokeEvent(EVENT e) {
			for (Listener& c : event_to_subscribers[e])
				c();
		}
		/*returns true if event is allowed*/
		bool isEventAllowed(EVENT) { return true; }

		/*adds subscriber to event. returns true if its the only subscriber*/
		bool AddListenerToEvent(EVENT e, Listener l) {
			auto& subs = event_to_subscribers[e];
			subs.push_back(l);
			return subs.size() == 1;
		}
		/*removes all subscribers to the event. returns the number of subscribers removed*/
		size_t ClearListenersToEvent(EVENT e) {
			auto& subs = event_to_subscribers[e];
			size_t ret = subs.size();
			subs.clear();
			return ret;
		}
	};

	namespace KeyBinds {
		BETTER_ENUM(BE_CONTROL_EVENT, int,
			DISPLAY_NEW_WINDOW,
			DISPLAY_TOGGLE_MOVEMENT_OVERLAY,
			MOVEMENT_ALIGN_TO,
			ENGR_INCREASE_PSU,
			ENGR_DECREASE_PSU,
			MOVEMENT_INCREASE_DRIVE,
			MOVEMENT_DECREASE_DRIVE
		);
		typedef BE_CONTROL_EVENT::_enumerated CONTROL_EVENT;

		struct KeyEventWrapperHasher {
			size_t operator()(const ftxui::Event& w) const {
				return std::hash<std::string>()(w.input());
			}
		};

		/*returns true if the key was accepted. false if it was denied or is not tied to anything*/
		size_t sendKey(ftxui::Event);
		/*returns true if event was added, false if already exists*/
		bool registerCtrlEvent(ftxui::Event, CONTROL_EVENT);
		/*returns true if event was removed, false if didnt exist*/
		void unregisterCtrlEvent(ftxui::Event, CONTROL_EVENT);

		extern Observer<CONTROL_EVENT> CtrlObserver;

		typedef std::unordered_map<ftxui::Event, std::vector<CONTROL_EVENT>, KeyEventWrapperHasher> Key2ControlEvents;
		extern Key2ControlEvents KeyMap;
	}
};
