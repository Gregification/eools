#include "Camera.hpp"

using namespace gs;

const Transformation_2D& Camera::getTransformationInverse() {
	if (c_d_trans != trans) {
		c_d_trans = trans;
		c_i_trans = trans.getInverse();
	}

	return c_i_trans;
}

inline float& Camera::offX() { return trans.transX(); }
inline float& Camera::offY() { return trans.transY(); }

Vec2_f Camera::getScaleVec() { return Vec2_f(trans.scaleX(), trans.scaleY()); }
Vec2_f Camera::getOffVec()	 { return Vec2_f(offX(), offY()); }

void Camera::Draw(Canvas& c, std::shared_ptr<Grid> g) {
	formerSize.x = c.width();
	formerSize.y = c.height();

	//track
	if (auto p = tracking.lock()) {
		const Vec2 sp = formerSize * .5 - gridToScreen(p->transform.position);
		offX() += sp.x;
		offY() += sp.y;
	}

	//debug
	static const Vec2_i size{ 50 };
	Vec2_i pos((c.width() - size.x) * .5f , (c.height() - size.y) * .5f );
	c.DrawText(20, 20, "drawing grid w/ id: " + std::to_string(g->id()));
	c.DrawText(20, 30, "# children: " + std::to_string(g->_go_vec.size()));
	c.DrawText(pos.x, pos.y,      "scale: "  + (std::string)getScaleVec());
	c.DrawText(pos.x, pos.y + 10, "offset: " + (std::string)getOffVec());
	c.DrawText(pos.x, pos.y + 20, "mouse: "  + (std::string)mouse_screen);
	c.DrawText(pos.x, pos.y + 40, "gtos: "   + (std::string)gridToScreen(screenToGrid(mouse_screen)));
	c.DrawText(pos.x, pos.y + 30, "stog: "   + (std::string)screenToGrid(mouse_screen));

	int y = 30;
	for (auto& v : g->_go_vec) {
		c.DrawText(25, y += 10, "id: " + std::to_string(v.id) + "?" + (v.go ? ("exists@(" + std::to_string(v.go->transform.position.x) + "," + std::to_string(v.go->transform.position.y) + ")") : "dne"));
		if (!v.go) continue;

		//draw the AABB
		/*{
			auto aabb = v.go->getAABB();
			aabb.size = gridToScreen(aabb.pos + aabb.size + v.go->transform.position);
			aabb.pos = gridToScreen(aabb.pos + v.go->transform.position);
			aabb.size -= aabb.pos;
			Vec2 tl = aabb.pos;
			Vec2 tr = aabb.pos; tr.x += aabb.size.x;
			Vec2 bl = aabb.pos; bl.y += aabb.size.y;
			Vec2 br = aabb.pos + aabb.size;
			//-
			c.DrawPointLine(tl.x, tl.y, tr.x, tr.y, Color::Green);
			// |
			c.DrawPointLine(tr.x, tr.y, br.x, br.y, Color::Green);
			//_
			c.DrawPointLine(bl.x, bl.y, br.x, br.y, Color::Green);
			//| 
			c.DrawPointLine(bl.x, bl.y, tl.x, tl.y, Color::Green);
			//loss
		}*/

		v.go->Draw(c, trans);		
	}

	//draw point at 0,0
	auto gp = trans.applyTo(Vec2_f(0));
	c.DrawBlock(gp.x, gp.y, true);
}
