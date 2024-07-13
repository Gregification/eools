#include "Body.hpp"

using namespace gs;

void Body::updateCoverage() {
	#define v verticies
	#define c coverage

	if (c.getArea() == 0 && v.size() > 0) {
		c.pos = c.size = v[0];

		for (int i = 1; i < v.size(); i++) {
			#define a v[i]
			//std::max\min isnt happy with templates

			//c.pos : smallest x,y pair
			//c.size : largest x,y pair

			if (c.pos.x > a.x)
				c.pos.x = a.x;
			else if (c.size.x < a.x)
				c.size.x = a.x;

			if (c.pos.y > a.y)
				c.pos.y = a.y;
			else if (c.size.y < a.y)
				c.size.y = a.y;
		}

		c.size -= c.pos;
	}

	//adjust so that it can be rotated however
	// pythagorean
	c.size *= c.size;
	c.size *= 2;
	c.size.x = std::sqrtf(c.size.x);
	c.size.y = std::sqrtf(c.size.y);
}

gs::Rectangle Body::getCoverage() {
	if (coverage.getArea() == 0 && verticies.size() > 1)
		updateCoverage();

	return coverage;
}

Transformation_2D Body::effectiveTransformation() {
	if (transform.rotation != rotation_cached) {
		rotatedTransformation = nonRotatedTransformation;
		rotatedTransformation.rotateBy(transform.rotation);
		rotation_cached = transform.rotation;
	}

	return rotatedTransformation;
}
