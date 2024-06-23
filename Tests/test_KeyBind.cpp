#include <set>
#include <iostream>

#include "../src/better-enums/enum.h"
#include <catch2/catch_test_macros.hpp>

#include "../src/Game/KeyBinds.hpp"

TEST_CASE("keybinds are setup properly", "[keybinds][ui]") {

	SECTION("all control events should have a name and description", "[ui][keybinds]") {
		const KeyBinds::Control2StringPairs& infoMap = KeyBinds::getInfoMap();

		for (const KeyBinds::CONTROL_EVENT& c : KeyBinds::CONTROL_EVENT::_values()) {

			SECTION("entry should exist") {
				REQUIRE(infoMap.contains(c));
			}

			SECTION("entry name should be unique and non empty") {
				static std::set<std::string> names;
				const std::string name = KeyBinds::infoMap.find(c)->second.first;

				//is non empty
				REQUIRE(!name.empty());

				//is unique
				REQUIRE(names.insert(name).second);
			}
		}
	}
	//this is not a exaustive test. just to ensure its not total gribrish
	SECTION("complete key & subscriber functionality", "[keybinds]") {
		using ftxui::Event;

		KeyBinds::CONTROL_EVENT ctrl = KeyBinds::CONTROL_EVENT::DISPLAY_NEW_WINDOW;
		int testNum;
		KeyBinds::ControlCall 
			funcA = [&testNum](Event e) { testNum += 1; }, 
			funcB = [&testNum](Event e) { testNum *= 2; };
		Event //arbitrary keys chosen
			keyA = Event::Character("a"),
			keyB = Event::Character("b");
		
		//assume basic funciton works
		KeyBinds::ClearCtrlEvntsOfKeyEvnt(keyA);//remove defaults assocaited with that key
		KeyBinds::ClearCtrlEvntsOfKeyEvnt(keyB);//remove defaults assocaited with that key
		REQUIRE(KeyBinds::SubCtrlEvntToKeyEvnt(keyA, ctrl));

		SECTION("things get called if subscribed, and dont when unsubscribed. not accounting how many times its called.") {
			//true because is subscribing something that is not already subscribed
			REQUIRE(KeyBinds::SubToCtrlEvnt(ctrl, funcA));

			//should trigger the subscriber
			testNum = 1;
			KeyBinds::sendEvent(keyA);
			REQUIRE(testNum == 2);

			//1 because only 1 thing should be subbed
			REQUIRE(KeyBinds::ClearSubsOfCtrlEvnt(ctrl) == 1);

			//should be unsubscribed
			testNum = 1;
			KeyBinds::sendEvent(keyA);
			REQUIRE(testNum == 1);

			//mutiple keybinds should be called
			REQUIRE(KeyBinds::SubToCtrlEvnt(ctrl, funcA));
			REQUIRE(!KeyBinds::SubToCtrlEvnt(ctrl, funcB));
			testNum = 1;
			KeyBinds::sendEvent(keyA); //n+1 *2
			REQUIRE(testNum == 4);

			REQUIRE(KeyBinds::ClearSubsOfCtrlEvnt(ctrl) == 2);
		}

		SECTION("key-to-control binds can be added and removed as expected") {
			KeyBinds::ClearSubsOfCtrlEvnt(ctrl);
			KeyBinds::ClearCtrlEvntsOfKeyEvnt(keyA);
			KeyBinds::ClearCtrlEvntsOfKeyEvnt(keyB);

			//is the only function subbed
			REQUIRE(KeyBinds::SubToCtrlEvnt(ctrl, funcA));

			KeyBinds::SubCtrlEvntToKeyEvnt(keyA, ctrl);
			//should just be 1 call
			testNum = 1;
			KeyBinds::sendEvent(keyA);//+1
			REQUIRE(testNum == 2);

			KeyBinds::SubCtrlEvntToKeyEvnt(keyB, ctrl);
			//should just be 1 call
			testNum = 1;
			KeyBinds::sendEvent(keyB);//+1
			REQUIRE(testNum == 2);

			REQUIRE(KeyBinds::UnsubCtrlEvntToKeyEvnt(keyA, ctrl));
			//nothing happens
			testNum = 1;
			KeyBinds::sendEvent(keyA);//+1
			REQUIRE(testNum == 1);
			//should just be 1 call
			testNum = 1;
			KeyBinds::sendEvent(keyB);//+1
			REQUIRE(testNum == 2);

			REQUIRE(KeyBinds::UnsubCtrlEvntToKeyEvnt(keyB, ctrl));
			//nothing happens
			testNum = 1;
			KeyBinds::sendEvent(keyA);//+1
			REQUIRE(testNum == 1);

			//clean up
			REQUIRE(KeyBinds::ClearSubsOfCtrlEvnt(ctrl) == 1);
		}
	}
};