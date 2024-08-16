#include "IFHelm.hpp"

IFHelm::IFHelm() {
	auto rdTransform = Renderer([&]() {
		auto s = _ship.lock();

		if (!s)
			return vbox({text("!!! ship disappeared?")});

		return ftxui::vbox({
			text("pos:" + (std::string)s->transform.position),
			text("vls:" + (std::string)s->transform.velocity),
			text("agl vel:" + std::to_string(s->transform.angularVelocity)),
		});
	});

	controls = Container::Vertical({});

	Add(
		Container::Horizontal({
			Container::Vertical({
				rdTransform,
				getShipView(),
			}),
			controls
		})
	);
}

void IFHelm::setShip(std::shared_ptr<Ship> s) {
	_ship = s;

	controls->DetachAllChildren();
	if(s)
	{//repopulate controls
		{//drive throttle
			auto slider = ftxui::Slider<float>({
					.value = &s->driveTrain.accele_throttle,
					.min = 0.0f,
					.max = 1.0f,
					.increment = 0.05f
				});
			//slider->On

			controls->Add(slider);
		}
	}
}

std::weak_ptr<Ship> IFHelm::getShip() const {
	return _ship;
}

ftxui::Component IFHelm::getShipView() const {
	return  Renderer([&] {
		const static Vec2_i r(35), off(1);
		const static Vec2_i centered = r / 2;
		const static Vec2_i direciotnIndicator{centered.x, 0};

		auto s = _ship.lock();
		auto c = Canvas(r.x + off.x * 2 + 3, r.y + off.y * 2 + 2);

		if (s) {
			auto cov = s->getAABB();
			cov.pos.x = std::abs(cov.pos.x);

			//returns b scaled to r & rotated
			const auto getP = [cov, &s](Vec2_f b) -> Vec2_f {
				b = s->transform.rotation.applyTo(b / cov.size * r);
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

					c.DrawBlockLine(f.x, f.y, p.x, p.y, Color::Green);

					f = p;
				}
			}

		} else {
			c.DrawText(off.x + r.x / 2, off.y + r.y / 2, "*stuff.png");
		}

		return canvas(std::move(c));
		});
}