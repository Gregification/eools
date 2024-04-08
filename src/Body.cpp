#include "Body.hpp"

using namespace gs;

void Body::updateCoverage() {
	coverage.pos.x  = coverage.pos.y  = 0;
	coverage.size.x = coverage.size.y = 0;

	for (Vec2& v : verticies) {
		if (v.x < coverage.pos.x)
			coverage.pos.x = v.x;
		else if (v.x - coverage.pos.x > coverage.size.x)
			coverage.size.x = v.x;

		if (v.y < coverage.pos.y) 
			coverage.pos.y = v.y;
		else if (v.y - coverage.pos.y > coverage.size.y)
			coverage.size.y = v.y;
	}

	coverage.size.x++;
	coverage.size.y++;
}

Transformation_2D Body::effectiveTransformation() {
	if (rotation != rotation_cached) {
		rotatedTransformation = nonRotatedTransformation;
		rotatedTransformation.rotateBy(rotation);
		rotation_cached = rotation;
	}

	return rotatedTransformation;
}