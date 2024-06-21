#include "../better-enums/enum.h"
#include <catch2/catch_test_macros.hpp>


#include "../Game/KeyBinds.hpp"

//TEST_CASE("keybinds are setup properly", "[keybinds]") {
//	SECTION("control modes must all be registered") {//no bad. TODO:remove this test, replace with somethign actually useful
//		const KeyBinds::ControlGroup2ContolEvents& mode2ControlsMap = KeyBinds::getControls();
//
//		for (KeyBinds::CONTROL_MODE cm : KeyBinds::CONTROL_MODE::_values()) {
//			REQUIRE(mode2ControlsMap.find(cm) != mode2ControlsMap.end());
//		}
//
//	}
//}