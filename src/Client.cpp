#include "Client.hpp"

void Client::run(ScreenInteractive& screen) {
	//update connection state
	{
		ConnectionStatus stat = ConnectionStatus();
		stat.isQueue = true;

		net::message<NetMsgType> msg;
		msg.header.id = NetMsgType::ConnectionStatus;
		msg << stat;
		Send(msg);
	}

	//ui rendering
	float avgPackets = 0;
	int tab_index = 0;
	auto clientStats = Renderer([&] {
			return text(std::format("| ~PKTs:{:3.0f} | fps:{:2.0f} |", avgPackets, fps));
		});
	std::vector<std::string> tab_entries = {
			"control",
			"map",
			"cargo",
			"expansions"
		};
	auto tab_selection = Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated());
	auto tab_content = Container::Tab({
			Renderer_play(),
			Renderer_map(),
			Renderer_inventory(),
			Renderer_upgrades()
		},
		&tab_index);
	auto tab_with_mouse = CatchEvent(tab_content, [&](Event e) {
			onInput(std::move(e));
			return false;
		});
	auto main_container = Container::Vertical({
			Container::Horizontal({
				tab_selection,
				clientStats
			}),
			tab_with_mouse
		});
	auto main_renderer = Renderer(main_container, [&] {
		return vbox({
				hbox({
					tab_selection->Render(),
					clientStats->Render()
				}),
				tab_with_mouse->Render() | flex
			});
		});

	Loop loop(&screen, main_renderer);

	//main
	{
		using namespace std::chrono;

		time_point
			start	= steady_clock::now(),
			now		= start,
			lastPhysTime = start;

		const long long target = 1000 / 60;
		long long dt;

		float avgElapse = 1000 / target;
		const float weight = avgElapse < 10 ? 10 : avgElapse;

		while (!loop.HasQuitted()) {		//game loop	
			start = steady_clock::now();

			loop.RunOnce();
			float numPkt = Update();

			now = steady_clock::now();
			dt = duration_cast<milliseconds>(now - start).count();
			
			ship->PhysUpdate(dt / 1000.0);

			if (dt*2 < target)
				std::this_thread::sleep_for(milliseconds(target - dt*2));

			now = steady_clock::now();
			dt = duration_cast<milliseconds>(now - start).count();

			fps = 1000 / avgElapse;
			avgElapse = avgElapse - (avgElapse / weight) + dt / weight;
			avgPackets= avgPackets- (avgPackets/ weight) +numPkt/ weight;

			screen.PostEvent(Event::Custom);
		}
	}
}

void Client::OnMessage(net::message<NetMsgType> msg) {
	switch (msg.header.id) {
		case NetMsgType::Ping: {
			Ping ping = Ping();
			msg >> ping;

			if (ping.isComplete()) {
				m_connection->pingTime = ping.getTime();
				break;
			}

			ping.tag();

			msg << ping;
			Send(msg);
		}break;
		case NetMsgType::IDPartition: {
			IDPartition idp = IDPartition();
			msg >> idp;
			if(!idp.isBad())
				LOCAL_PARITION = idp;
		}break;
	}
}

Component Client::Renderer_play() {
	return Renderer([&] {
		return canvas([&](Canvas& c) {
			Draw(c);
		});
	});
}

Component Client::Renderer_map() {
	return Renderer([&] {
		return vbox({
				text("renderer map") | center
			});
		});
}

Component Client::Renderer_upgrades() {
	return Renderer([&] {
		return vbox({
				text("renderer upgrades") | center
			});
	});
}

Component Client::Renderer_inventory() {
	return Renderer([&] {
		return vbox({
				text("renderer inventory") | center
			});
	});
}

void Client::Draw(Canvas& c) {
	if (gridIsReady)
		;//currentGrid()->Draw(c, camOffset, scale);
	else {
		using namespace std::chrono;
		static time_point
			start = steady_clock::now(),
			now = start;
		now = steady_clock::now();
		const float dt = duration_cast<milliseconds>(start - now).count() / 1000.0;
		start = now;

		const int l = 6;
		int w = c.width(), h = c.height();
		int w2 = w / 2, h2 = h / 2;
		float o = 10 * dt;

		static float offX = 4, offY = 4, xs = 1, ys = 1;
		offX += o * xs;
		offY += o * ys;

		if(offX > w/l || offX < 3) xs *= -1;
		if(offY > h/l || offY < 3) ys *= -1;

		for (int i = 1; i < l; i++) {
			float r = 1 / i;

			c.DrawPointEllipse(
				w2 + (mouse.x - w2) *i*i / w2,
				h2 + (mouse.y - h2) *i*i / h2,
				offX * i,
				offY * i
			);
		}
	}
}

void Client::onInput(Event e) {
	if (e.is_mouse()) {
		mouse.x = (e.mouse().x - 1) * 2;
		mouse.y = (e.mouse().y - 1) * 4;

		switch (e.mouse().button) {
			case Mouse::Left:
				break;
			case Mouse::Middle:
				break;
			case Mouse::Right:
				break;
			case Mouse::WheelUp:
				scale += scale * 1.2;
				break;
			case Mouse::WheelDown:
				scale -= scale / 1.2;
				break;
		}
	}
}