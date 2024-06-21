#include <set>

#include "../src/better-enums/enum.h"
#include <catch2/catch_test_macros.hpp>

#include "../src/Game/KeyBinds.hpp"

TEST_CASE("keybinds are setup properly", "[keybinds][ui]") {
	const KeyBinds::Control2StringPair& infoMap = KeyBinds::getInfoMap();

	SECTION("all controls must have a name and description entry") {

		//# of controls match # of entries
		REQUIRE(infoMap.size() == KeyBinds::CONTROLS::_size());

		for (const KeyBinds::CONTROLS &c : KeyBinds::CONTROLS::_values()) {
			auto info_ = infoMap.find(c);

			//does entry exist
			REQUIRE(info_ != infoMap.end());
		}
	}

	SECTION("control names must be unique and non empty") {
		std::set<std::string> names;

		for (const KeyBinds::CONTROLS& c : KeyBinds::CONTROLS::_values()) {
			std::string name = infoMap.find(c)->second.first;

			//name is non empty
			REQUIRE(!name.empty());

			//name is unique
			REQUIRE(names.insert(name).second);
		}
	}
}