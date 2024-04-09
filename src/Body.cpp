#include "Body.hpp"

using namespace gs;

void Body::updateCoverage() {

	// TODO: readjust points for com at (0,0)
	// https://en.wikipedia.org/wiki/Centroid
	int max = 0;
	for (int i = 0; i < verticies.size(); i++) {
		int tmp = verticies[i].magnitudeSquared();
		if (tmp > max) max = tmp;
	}

	max = std::sqrt(max);
	coverage.pos.x  = coverage.pos.y  = -max;
	coverage.size.x = coverage.size.y = max+1;
}

Transformation_2D Body::effectiveTransformation() {
	if (transform.rotation != rotation_cached) {
		rotatedTransformation = nonRotatedTransformation;
		rotatedTransformation.rotateBy(transform.rotation);
		rotation_cached = transform.rotation;
	}

	return rotatedTransformation;
}