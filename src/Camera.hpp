#pragma once

#include "GameObjects/Grid.hpp"

//TODO: some way to cache the inverse transformation

class Camera {
public:
	Camera() : mouse_screen(0), trans{Transformation_2D::identity} {

	}
	~Camera() = default;
	
	Vec2_i mouse_screen;

	inline float& offX();
	inline float& offY();

	Vec2_f getScaleVec();
	Vec2_f getOffVec();

	void Draw(Canvas& c, std::shared_ptr<Grid> g);

	Vec2 screenToGrid(Vec2_i);
	Vec2_i gridToScreen(Vec2);

	Transformation_2D trans;
};