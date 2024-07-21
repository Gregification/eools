#include "Camera.hpp"

using namespace gs;

inline float& Camera::offX() { return trans.transX(); }
inline float& Camera::offY() { return trans.transY(); }

Vec2_f Camera::getScaleVec() { return Vec2_f(trans.scaleX(), trans.scaleY()); }
Vec2_f Camera::getOffVec()	 { return Vec2_f(offX(), offY()); }

void Camera::Draw(Canvas& c, std::shared_ptr<Grid> g) {
	Vec2_i
		size(50),
		pos((c.width() - size.x) / 2 , (c.height() - size.y) / 2),
		tl(pos),
		tr(pos.x + size.x, pos.y),
		bl(pos.x, pos.y + size.y),
		br(pos.x + size.x, pos.y + size.y)
	;
	
	/*tl = trans.applyTo(tl);
	bl = trans.applyTo(bl);
	tr = trans.applyTo(tr);
	br = trans.applyTo(br);
	c.DrawPointLine(bl.x, bl.y, tl.x, tl.y, Color::Green);
	c.DrawPointLine(bl.x, bl.y, br.x, br.y, Color::Green);
	c.DrawPointLine(tr.x, tr.y, br.x, br.y, Color::Green);
	c.DrawPointLine(tr.x, tr.y, tl.x, tl.y, Color::Green);
	c.DrawPointLine(tl.x, tl.y, br.x, br.y, Color::Blue);
	c.DrawPointLine(bl.x, bl.y, tr.x, tr.y, Color::Orange1);*/

	c.DrawText(20, 20, "drawing grid w/ id: " + std::to_string(g->id()));
	c.DrawText(20, 30, "# children: " + std::to_string(g->_go_vec.size()));

	c.DrawText(pos.x, pos.y, "scale: " + (std::string)getScaleVec());
	c.DrawText(pos.x, pos.y + 10, "offset: " + (std::string)getOffVec());
	c.DrawText(pos.x, pos.y + 20, "mouse: " + (std::string)mouse_screen);
	c.DrawText(pos.x, pos.y + 30, "stog: " + (std::string)screenToGrid(mouse_screen));
	c.DrawText(pos.x, pos.y + 40, "gtos: " + (std::string)gridToScreen(screenToGrid(mouse_screen)));

	int y = 30;
	for (auto& v : g->_go_vec) {
		c.DrawText(25, y += 10, "id: " + std::to_string(v.id) + "?" + (v.go ? ("exists@(" + std::to_string(v.go->transform.position.x) + "," + std::to_string(v.go->transform.position.y) + ")") : "dne"));
		if (!v.go) continue;

		v.go->Draw(c, trans);		
	}
	auto gp = trans.applyTo(Vec2(0));
	c.DrawBlock(gp.x, gp.y, true);
}

Vec2 Camera::screenToGrid(Vec2_i p) {
	return trans.getInverse().applyTo(static_cast<Vec2>(p));
}

Vec2_i Camera::gridToScreen(Vec2 p) {
	return (Vec2_i)(trans.applyTo(p) - getOffVec());
}
