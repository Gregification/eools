#include "Camera.hpp"

using namespace gs;

void Camera::Draw(Canvas& c, std::shared_ptr<Grid> g) {
	//basic tranform
	Transformation_2D t;
	t.transX() = offset.x;
	t.transY() = offset.y;
	t.scaleX() = t.scaleY() = scale;

	//center camera back on what ever was being centered
	Vec2_f no = t.applyTo(offset);

	Vec2_i
		size(50),
		pos((c.width() - size.x) / 2 , (c.height() - size.y) / 2),
		tl(pos),
		tr(pos.x + size.x, pos.y),
		bl(pos.x, pos.y + size.y),
		br(pos.x + size.x, pos.y + size.y)
	;

	tl = t.applyTo(tl);
	bl = t.applyTo(bl);
	tr = t.applyTo(tr);
	br = t.applyTo(br);

	c.DrawPointLine(bl.x, bl.y, tl.x, tl.y, Color::Green);
	c.DrawPointLine(bl.x, bl.y, br.x, br.y, Color::Green);
	c.DrawPointLine(tr.x, tr.y, br.x, br.y, Color::Green);
	c.DrawPointLine(tr.x, tr.y, tl.x, tl.y, Color::Green);

	c.DrawPointLine(tl.x, tl.y, br.x, br.y, Color::Blue);
	c.DrawPointLine(bl.x, bl.y, tr.x, tr.y, Color::Orange1);

	//c.DrawBlockCircleFilled(mouse.x, mouse.y, 9);

	c.DrawText(20, 20, "drawing grid w/ id: " + std::to_string(g->id()));
	c.DrawText(20, 30, "# children: " + std::to_string(g->_go_vec.size()));

	c.DrawText(pos.x, pos.y, "scale: " + std::to_string(scale));
	c.DrawText(pos.x, pos.y + 10, "offset: " + (std::string)offset);

	int y = 30;
	for (auto& v : g->_go_vec) {
		c.DrawText(25, y += 10, "id: " + std::to_string(v.id) + "?" + (v.go ? ("exists@(" + std::to_string(v.go->transform.position.x) + "," + std::to_string(v.go->transform.position.y) + ")") : "dne"));
		if (!v.go) continue;

		v.go->Draw(c, t);
	}
}

Vec2 Camera::screenToGrid(Vec2_i p) const
{
	p /= scale;
	return (Vec2)(p - offset);
}

Vec2_i Camera::gridToScreen(Vec2 p) const
{
	return (Vec2_i)(p + offset);
}
