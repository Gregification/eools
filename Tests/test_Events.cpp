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

	Events::Observer<_ENUM::_enumerated> obs{{}};

	SECTION("all key binds have name and description") {
		using namespace Events::KeyBinds;
		auto& infoMap = observer.event_name_and_description;

		SECTION("entries should exist") {
			for (int i = 0; i < BE_CONTROL_EVENT::_size(); i++)
				REQUIRE(infoMap.contains(BE_CONTROL_EVENT::_from_index(i)));

			REQUIRE(BE_CONTROL_EVENT::_size() == infoMap.size());
		}
	};

	SECTION("no type functions") {
		
		int testNum = 1;
		auto la = std::make_shared<Events::Listener<>>([&] { testNum++; });
		auto lb = std::make_shared<Events::Listener<>>([&] { testNum *= 2; });

		REQUIRE(obs.AddListenerToEvent(_ENUM::_enumerated::A, la));
		REQUIRE(!obs.AddListenerToEvent(_ENUM::_enumerated::A, la));
		
		testNum = 1;
		obs.forceInvokeEvent(_ENUM::_enumerated::A);
		REQUIRE(testNum == 3);

		REQUIRE(obs.isEventAllowed(_ENUM::_enumerated::A) == obs.invokeEvent(_ENUM::_enumerated::A));

		REQUIRE(obs.ClearListenersToEventOfType(_ENUM::_enumerated::A) == 2);

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

		REQUIRE(obs.ClearListenersToEventOfType(_ENUM::_enumerated::A) == 2);
		
	}

	SECTION("typed functions") {
		int testNum = 1;
		struct demoStruct {
			int val = -999;
			std::string meow;
		};

		auto lint = std::make_shared<Events::Listener<int>>([&](int) { testNum++; });
		auto lstruct = std::make_shared<Events::Listener<demoStruct>>([&](demoStruct s) { testNum = s.val; });

		SECTION("listeners associated to arguement type") {
			REQUIRE(obs.AddListenerToEvent(_ENUM::_enumerated::A, lint));
			REQUIRE(obs.AddListenerToEvent(_ENUM::_enumerated::A, lstruct));

			testNum = 1;
			obs.forceInvokeEvent<int>(_ENUM::_enumerated::A);
			REQUIRE(testNum == 2);

			obs.forceInvokeEvent<demoStruct>(_ENUM::_enumerated::A);
			REQUIRE(testNum == demoStruct{}.val);
		}
	 }
};
