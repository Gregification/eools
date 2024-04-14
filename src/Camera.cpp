#include "Camera.hpp"

void Camera::Draw(Canvas& c, std::shared_ptr<Grid> g) {
	gs::Transformation_2D t;
	t.offX() = offset.x;
	t.offY() = offset.y;
	t.scaleX() = t.scaleY() = scale;

	g->Draw(c, t);
}
