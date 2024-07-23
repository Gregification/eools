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
	std::vector<gs::Vec2_f> verticies{
		{ 0,  5},
		{ 5,  -5},
		{ 0,  -2},
		{ -5, -5}
	};

	gs::Transformation_2D nonRotatedTransformation;

	void recalAABB();
	gs::Rectangle getAABB();

	gs::Transformation_2D effectiveTransformation();

	bool isDetected() { return true; }

	bool isPointInBody(gs::Vec2_f) const;

private:
	gs::Rectangle coverage;//rough area that the verticies covers

	float rotation_cached;
	gs::Transformation_2D rotatedTransformation;
	//https://stackoverflow.com/questions/7469959/given-2-points-how-do-i-draw-a-line-at-a-right-angle-to-the-line-formed-by-the-t/7470098#7470098
};