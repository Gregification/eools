#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include <ftxui/component/event.hpp>
#include "../../better-enums/enum.h"

namespace Events {
	//template jungle
	typedef int _DEFAULT_TYPE;

	template<typename T = _DEFAULT_TYPE>
	struct WrappedArgument {
		T value;
		using Type = T;
	};

	template<typename ARG = _DEFAULT_TYPE>
	struct Listener {
		typedef WrappedArgument<ARG> WrappedArg;

	public:
		Listener(std::function<void()> callback) :
			_callback([=](WrappedArg&) { callback(); })
		{}
		Listener(std::function<void(ARG)> callback) :
			_callback([=](WrappedArg& arg) { callback(arg.value); })
		{}

		void Run(WrappedArg& arg) { _callback(arg); }
	private:
		std::function<void(WrappedArg)> _callback;
	};

	template<typename EVENT> //should be enum. DO NOT USE BETTER_ENUM
	class Observer {
	private:
		typedef std::unordered_map<EVENT, std::vector<Listener<>>>
			Event2Subs;
		typedef std::unordered_map<EVENT, const std::pair< const std::string, const std::string>>
			Event2StringPair;

		Event2Subs event_to_subscribers;

	public:
		Observer(Event2StringPair e) : event_name_and_description(e) {}
		const Event2StringPair event_name_and_description;

		/*triggers a event if it's allowed. returns same as KeyBinds::isEventAllowed*/
		template<typename ARG = _DEFAULT_TYPE>
		bool invokeEvent(EVENT e, ARG arg = 0) {
			if (!isEventAllowed(e)) return false;

			forceInvokeEvent<ARG>(std::move(e), arg);

			return true;
		}
		/*guarentted to trigger events with matching arguemetns*/
		template<typename ARG = _DEFAULT_TYPE>
		void forceInvokeEvent(EVENT e, ARG arg = NULL) {

			auto& listeners = event_to_subscribers[std::move(e)];
			auto warg = WrappedArgument<ARG>{ arg };

			for (auto& c : listeners) {
				c.Run(warg);
			}
		}
		/*returns true if event is allowed*/
		bool isEventAllowed(EVENT) { return true; }

		/*adds subscriber to event. returns true if its the only subscriber*/
		template<typename ARG = _DEFAULT_TYPE>
		bool AddListenerToEvent(EVENT e, Listener<ARG> l) {
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

		struct _WrapperHasher {
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

		extern Observer<CONTROL_EVENT> observer;

		typedef std::unordered_map<ftxui::Event, std::vector<CONTROL_EVENT>, _WrapperHasher> Key2ControlEvents;
		extern Key2ControlEvents KeyMap;
	}

	namespace ClientEvent {
		BETTER_ENUM(BE_CLIENT_EVENT, int, 
			EVENT_MESSAGE	
		);
		typedef BE_CLIENT_EVENT::_enumerated CLIENT_EVENT;

		extern Observer<CLIENT_EVENT> observer;
	}
};
