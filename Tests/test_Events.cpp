#include <set>
#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include "../src/Game/Events/Events.hpp"

BETTER_ENUM(_ENUM, int,
	A,
	B,
	C
);
//enum _ENUM {
//	A,
//	B,
//	C
//};

TEST_CASE("event observer works as intended", "[keybinds][ui]") {
	//these are not a exaustive test. just to ensure its not total gribrish

	int testNum = 1;
	const auto la = [&] { testNum++; };
	const auto lb = [&] { testNum *= 2; };
	Events::Observer<_ENUM::_enumerated> obs{ {} };
	
	REQUIRE(obs.AddListenerToEvent(_ENUM::_enumerated::A, la));
	REQUIRE(!obs.AddListenerToEvent(_ENUM::_enumerated::A, la));

	testNum = 1;
	obs.forceInvokeEvent(_ENUM::_enumerated::A);
	REQUIRE(testNum == 3);

	REQUIRE(obs.isEventAllowed(_ENUM::_enumerated::A) == obs.invokeEvent(_ENUM::_enumerated::A));

	REQUIRE(obs.ClearListenersToEvent(_ENUM::_enumerated::A) == 2);

	testNum = 1;
	obs.forceInvokeEvent(_ENUM::A);
	REQUIRE(testNum == 1);

	REQUIRE(obs.AddListenerToEvent(_ENUM::_enumerated::A, la));
	REQUIRE(!obs.AddListenerToEvent(_ENUM::_enumerated::A, lb));

	testNum = 1;
	obs.forceInvokeEvent(_ENUM::_enumerated::C);
	REQUIRE(testNum == 1);
	
	testNum = 0;
	obs.forceInvokeEvent(_ENUM::_enumerated::A);
	REQUIRE(testNum == 2);

	SECTION("events all have name and description") {
		using namespace Events::KeyBinds;
		auto& infoMap = CtrlObserver.event_name_and_description;

		SECTION("entries should exist") {
			for (int i = 0; i < BE_CONTROL_EVENT::_size(); i++)
				REQUIRE(infoMap.contains(BE_CONTROL_EVENT::_from_index(i)));

			REQUIRE(BE_CONTROL_EVENT::_size() == infoMap.size());
		}
	};
};
