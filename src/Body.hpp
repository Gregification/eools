#pragma once

#include <array>

#include "GameStructs.hpp"

//physics & game-stat related aspects of a gameobject
class Body {
public:
	Body() :transform(),
			rotation_cached(),
			nonRotatedTransformation(gs::Transformation_2D::identity),
			rotatedTransformation(gs::Transformation_2D::identity) 
		{}
	virtual ~Body() = default;

public:
	long long lastUpdate;
	Unit mass = .01 * KT;

	gs::Transform transform;
	float completeScale = 1;
	std::vector<gs::Vec2> verticies;//no concave, plz. i beg

	gs::Rectangle coverage;//rough area that the verticies covers
	gs::Transformation_2D nonRotatedTransformation;

	void updateCoverage();
	gs::Transformation_2D effectiveTransformation();

	bool isDetected() { return true; }

private:
	float rotation_cached;
	gs::Transformation_2D rotatedTransformation;
	//https://stackoverflow.com/questions/7469959/given-2-points-how-do-i-draw-a-line-at-a-right-angle-to-the-line-formed-by-the-t/7470098#7470098
};