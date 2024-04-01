#pragma once

#include <ftxui/component/component.hpp>

#include "GameStructs.hpp"
#include "Ship.hpp"

using namespace ftxui;

class ShipBuilder {
	public:
		ShipBuilder() : maxDimension({15,15}) {

		}

		Component getRenderer();

	public:
		std::shared_ptr<Ship> ship;

		Vec2 maxDimension;

};