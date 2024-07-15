#pragma once

#include "GameObjects/Grid.hpp"

class Camera {
public:
	Camera() : mouse_screen(0), trans{Transformation_2D::identity} {

	}
	~Camera() = default;
	
	Transformation_2D trans;
	Vec2_i mouse_screen;

	inline float& offX();
	inline float& offY();

	Vec2_f getScaleVec();
	Vec2_f getOffVec();

	void Draw(Canvas& c, std::shared_ptr<Grid> g);
	
	Vec2 screenToGrid(Vec2_i);
	Vec2_i gridToScreen(Vec2);
};