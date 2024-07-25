#include "IFHelm.hpp"

IFHelm::IFHelm() {
	auto rdTransform = Renderer([&]() {
		auto s = _ship.lock();

		return ftxui::vbox({
			text("pos:" + (std::string)s->transform.position),
			text("vls:" + (std::string)s->transform.velocity),
			text("agl:" + std::to_string(s->transform.angularVelocity)),
		});
	});

	Add(rdTransform);
}

void IFHelm::setShip(std::shared_ptr<Ship> sp) {
	_ship = sp;
}

std::weak_ptr<Ship> IFHelm::getShip() const {
	return _ship;
}

ftxui::Component IFHelm::getShipView() const {
	return  Renderer([&] {
		const static Vec2_i r(35), off(1);

		auto s = _ship.lock();
		auto c = Canvas(r.x + off.x * 2 + 3, r.y + off.y * 2 + 2);

		if (s) {
			auto cov = s->getAABB();
			cov.pos.x = std::abs(cov.pos.x);
			Vec2_i centered = r / 2;

			//returns b scaled to r
			const auto getP = [cov, centered](Vec2_f b) -> Vec2_f {
				b = b / cov.size * r;
				b += centered + off;
				return b;
				};

			if (cov.getArea() == 0) {
				c.DrawBlock(r.x / 2 + off.x, r.y / 2 + off.y, true);
			}
			else {
				const auto& v = s->verticies;

				Vec2_f f = getP((Vec2_f)v[0]);

				for (int i = 1; i < v.size() + 1; i++) {
					Vec2_f p = getP((Vec2_f)v[i % v.size()]);

					c.DrawBlockLine(f.x, f.y, p.x, p.y, Color::Red);

					f = p;
				}
			}
		}
		else {
			c.DrawText(off.x + r.x / 2, off.y + r.y / 2, "*stuff.png");
		}

		return canvas(std::move(c));
		});
}