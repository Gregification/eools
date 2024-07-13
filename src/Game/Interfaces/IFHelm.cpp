#include "IFHelm.hpp"

IFHelm::IFHelm() {
	auto shipView = Renderer([&] {
		const static Vec2_i r(70, 70), off(5,5);

		auto s = _of.lock();
		auto c = Canvas(r.x+off.x * 2, r.y+off.y * 2);

		c.DrawPointLine(off.x, off.y, off.x + r.x, off.y, Color::Green);
		c.DrawPointLine(off.x + r.x, off.y + r.y, off.x + r.x, off.y, Color::Green);
		c.DrawPointLine(off.x + r.x, off.y + r.y, off.x, off.y + r.y, Color::Green);
		c.DrawPointLine(off.x, off.y, off.x, off.y + r.y, Color::Green);

		if (s) {
			const auto cov = s->getCoverage();

			if (cov.getArea() == 0) {
				c.DrawBlock(r.x / 2 + off.x, r.y / 2 + off.y, true);
			}
			else {
				const auto& v = s->verticies;
				auto mod = s->effectiveTransformation();
				
				auto f = v[0] / cov.size * r + off;

				for (int i = 1; i < v.size() + 1; i++) {
					auto p = v[i % v.size()] / cov.size * r + off;
					
					c.DrawBlockLine(f.x, f.y, p.x, p.y, Color::Red);

					f = p;
				}
			}
		}
		else {
			c.DrawText(off.x, off.y, "*stuff.png");
		}
		
		return canvas(std::move(c));
	});
	
	Add(shipView);
}

void IFHelm::Of(std::shared_ptr<Ship> sp) {
	_of = sp;
}

std::weak_ptr<Ship> IFHelm::getOf() const {
	return _of;
}