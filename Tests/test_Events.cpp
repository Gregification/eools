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

	SECTION("events all have name and description") {
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
		Events::Listener
			la([&] { testNum++; }),
			lb([&] { testNum *= 2; });

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

		REQUIRE(obs.ClearListenersToEvent(_ENUM::_enumerated::A) == 2);
		
	}

	SECTION("typed functions") {
		int testNum = 1;
		struct demoStruct {
			int val = -999;
			std::string meow;
		};

		Events::Listener lint([&](int b){ testNum++; });
		//Events::Listener lstruct([&](demoStruct s) { testNum = s.val; });

		SECTION("listeners associated to arguement type") {
			//REQUIRE(obs.AddListenerToEvent(_ENUM::_enumerated::A, [&](int b) { testNum++; }));

			testNum = 1;
			obs.forceInvokeEvent<demoStruct>(_ENUM::_enumerated::A);
			//REQUIRE(testNum == 2);
		}
	 }
};
