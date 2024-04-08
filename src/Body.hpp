#pragma once

#include <array>

#include "GameStructs.hpp"

class Body {
public:
	Body() : 
			rotation(0),
			rotation_cached(-1),
			nonRotatedTransformation(gs::Transformation_2D::identity),
			rotatedTransformation(gs::Transformation_2D::identity) {
		
	}
	virtual ~Body() = default;

public:
	std::vector<gs::Vec2> verticies;//no concave, plz. i beg
	gs::Rectangle coverage;//rough area that the verticies covers

	float rotation;
	gs::Transformation_2D nonRotatedTransformation;

public:
	void updateCoverage();
	gs::Transformation_2D effectiveTransformation();

protected:
	float rotation_cached;
	gs::Transformation_2D rotatedTransformation; //cache
};