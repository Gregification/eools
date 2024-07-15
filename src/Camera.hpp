#pragma once

#include "GameObjects/Grid.hpp"

class Camera {
public:
	Camera() : offset(0), mouse_screen(0), scale(1) {

	}
	~Camera() = default;
	
	gs::Vec2 offset;
	gs::Vec2_i mouse_screen;
	float scale;

	void Draw(Canvas& c, std::shared_ptr<Grid> g);
	
	Vec2 screenToGrid(Vec2_i) const;
	Vec2_i gridToScreen(Vec2) const;
};