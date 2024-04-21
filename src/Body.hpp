#pragma once

#include <array>

#include "GameStructs.hpp"

class Body {
public:
	Body() :transform(),
			rotation_cached(),
			nonRotatedTransformation(gs::Transformation_2D::identity),
			rotatedTransformation(gs::Transformation_2D::identity) 
		{}
	virtual ~Body() = default;

public:
	go_time lastUpdate;
	float mass = INFINITY;

	gs::Transform transform;
	float completeScale = 1;
	std::vector<gs::Vec2> verticies;//no concave, plz. i beg

	gs::Rectangle coverage;//rough area that the verticies covers
	gs::Transformation_2D nonRotatedTransformation;

public:
	void updateCoverage();
	gs::Transformation_2D effectiveTransformation();

private:
	float rotation_cached;
	gs::Transformation_2D rotatedTransformation;
};