#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <variant>
#include <any>
#include <typeindex>
#include <memory>

#include <ftxui/component/event.hpp>
#include "../../better-enums/enum.h"

namespace Events {

	//cheesy way to have a void parameter
	struct DiscountVoid{};
	typedef DiscountVoid _DEFAULT_TYPE;

	struct ListenerBase { virtual ~ListenerBase() = default; };

	template<typename ARG = DiscountVoid>
	struct Listener : ListenerBase {
	public:
		typedef std::function<void(ARG)> WrappedFunc;
		WrappedFunc _callback;

		Listener(std::function<void()> callback)
			: _callback([=](ARG) { callback(); }) {}
		Listener(WrappedFunc callback)
			: _callback([=](ARG arg) { callback(std::move(arg)); }) {}
		//cant figure out how to get some sort of dynmaic cast action, i give up

		void Run(ARG arg) { _callback(std::move(arg)); }
	};

	template<typename EVENT>
	class Observer {
	private:

		typedef std::unordered_map<std::type_index, std::vector<std::shared_ptr<ListenerBase>>>
			SubGroup2Listeners;
		typedef std::unordered_map<EVENT, SubGroup2Listeners>
			Event2SubGroup;
		typedef std::unordered_map<EVENT, const std::pair< const std::string, const std::string>>
			Event2StringPair;

		Event2SubGroup event_to_subscribers;

	public:
		Observer(Event2StringPair e) : event_name_and_description(e) {}
		const Event2StringPair event_name_and_description;

		/*triggers a event if it's allowed. returns same as KeyBinds::isEventAllowed*/
		template<typename ARG = _DEFAULT_TYPE>
		bool invokeEvent(EVENT e, ARG arg = ARG()) {
			if (!isEventAllowed(e)) return false;

			forceInvokeEvent<ARG>(std::move(e), std::move(arg));

			return true;
		}

		/*guarentted to trigger events with matching arguemetns. returns # of triggered events*/
		template<typename ARG = _DEFAULT_TYPE>
		size_t forceInvokeEvent(EVENT e, ARG arg = ARG()) {
			SubGroup2Listeners& subgroup = event_to_subscribers[std::move(e)];
			auto& arr = subgroup[typeid(ARG)];

			for (auto lb : arr) {
				auto l = dynamic_pointer_cast<Listener<ARG>>(lb);

				l->Run(arg); 
			}

			return arr.size();
		}
		/*returns true if event is allowed*/
		bool isEventAllowed(EVENT) { return true; }

		/*adds subscriber to event. returns true if its the only subscriber*/
		template<typename ARG>
		bool AddListenerToEvent(EVENT e, std::shared_ptr<Listener<ARG>> l) {
			SubGroup2Listeners& subgroup = event_to_subscribers[std::move(e)];
			auto& arr = subgroup[typeid(ARG)];

			arr.push_back(std::move(l));

			return arr.size() == 1;
		}
		/*removes all subscribers to the event. returns the number of subscribers removed*/
		template<typename ARG = _DEFAULT_TYPE>
		size_t ClearListenersToEventOfType(EVENT e) {
			SubGroup2Listeners& subgroup = event_to_subscribers[std::move(e)];
			auto& arr = subgroup[typeid(ARG)];

			size_t ret = arr.size();
			arr.clear();

			return ret;
		}

		/*returns # of types removed*/
		size_t ClearListenersToEvent(EVENT e) {
			SubGroup2Listeners& subgroup = event_to_subscribers[std::move(e)];

			size_t ret = subgroup.count();
			subgroup.clear();

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
