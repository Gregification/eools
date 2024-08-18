#include "IFHelm.hpp"

#include <ftxui/component/component.hpp>

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
	{
		controls->Add(GenControls(*s));
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

Component IFHelm::GenControls(Ship& s)
{
	auto drive_throttle = ftxui::Slider("meow",
		&s.driveTrain.accele_throttle,
		0.0f,
		100.0f,
		2.5f);

	return {
		drive_throttle,
	};
}
