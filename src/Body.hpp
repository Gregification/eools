#pragma once

#include <array>

#include "GameStructs.hpp"

//physics & game-stat related aspects of a gameobject
class Body {
public:
	Body() :transform()
	{}
	virtual ~Body() = default;

public:
	long long lastUpdate;
	Unit mass = .01 * KT;

	gs::Transform transform;

	std::vector<gs::Vec2_f> verticies{
		{  0.00 * KM,  0.05 * KM},
		{  0.05 * KM, -0.05 * KM},
		{  0.00 * KM, -0.02 * KM},
		{ -0.05 * KM, -0.05 * KM}
	};

	void recalAABB();
	gs::Rectangle getAABB();

	bool isDetected() { return true; }

	bool isPointInBody(gs::Vec2_f) const;

private:
	gs::Rectangle coverage;//rough area that the verticies covers

	//https://stackoverflow.com/questions/7469959/given-2-points-how-do-i-draw-a-line-at-a-right-angle-to-the-line-formed-by-the-t/7470098#7470098
};