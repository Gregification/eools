#pragma once

#include "ShipBuilder.hpp"

Component ShipBuilder::getRenderer() {
	return Renderer([&] {
		return text("ship builder renderer");
	});
}