#pragma once

#include <array>

#include "GameStructs.hpp"

//physics & game-stat related aspects of a gameobject
class Body {
public:
	Body() :transform(),
			rotation_cached(),
			nonRotatedTransformation(gs::Transformation_2D::identity),
			rotatedTransformation(gs::Transformation_2D::identity),
			coverage({ 0,0 }, {0,0})
	{}
	virtual ~Body() = default;

public:
	long long lastUpdate;
	Unit mass = .01 * KT;

	gs::Transform transform;
	std::vector<gs::Vec2_i> verticies{ {-5,-5}, {-5, 5}, {5, 5}, {5, -5} };//no concave, plz. i beg

	gs::Transformation_2D nonRotatedTransformation;

	void updateCoverage();
	gs::Rectangle getCoverage();

	gs::Transformation_2D effectiveTransformation();

	bool isDetected() { return true; }

private:
	gs::Rectangle coverage;//rough area that the verticies covers

	float rotation_cached;
	gs::Transformation_2D rotatedTransformation;
	//https://stackoverflow.com/questions/7469959/given-2-points-how-do-i-draw-a-line-at-a-right-angle-to-the-line-formed-by-the-t/7470098#7470098
};