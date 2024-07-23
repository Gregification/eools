#include "../src/better-enums/enum.h"
#include <catch2/catch_test_macros.hpp>

#include "../src/Game/Navigation.hpp"

using namespace Navigation;

typedef TRAVEL_STATE TS;
typedef TRAVEL_STATE::_enumerated TS_E;

TEST_CASE("all states & naviagtor paired", "[game][player]") {
	SECTION("all states paired to a navigator") {

		for (int i = 0; i < TS::_size(); i++) {
			auto it = stateToNavigator.find(TS::_from_index(i));

			REQUIRE(it != stateToNavigator.end());
		}
	}
}