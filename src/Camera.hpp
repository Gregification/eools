#pragma once

//not really a camera. more so for organization
/**
* acts and render manager. controls where and how to draw GameObject's 
	on a canvas from a given Grid, scale, and offset(can be negative).
	uses affine transformations. -> https://en.wikipedia.org/wiki/Affine_transformation
*/
#include "Grid.hpp"

class Camera {
public:
	Camera() {

	}
	~Camera() = default;

public:
	Vec2 offset;
	float scale;

	void Draw(Canvas& c, Grid& g);

};