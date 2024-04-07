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
			if (e.is_mouse()) {
				mouse.x = e.mouse().x;
				mouse.y = e.mouse().y;
			}
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

	//main loop
	{
		using namespace std::chrono;

		time_point 
			start = steady_clock::now(),
			now = steady_clock::now();

		const long long target = 1000 / 60;
		long long dt;

		float avgElapse = 1000 / target;
		const float weight = avgElapse < 10 ? 10 : avgElapse;

		while (!loop.HasQuitted()) {			
			start = steady_clock::now();

			loop.RunOnce();
			float numPkt = Update();

			now = steady_clock::now();
			dt = duration_cast<milliseconds>(now - start).count();

			if (dt*2 < target)
				std::this_thread::sleep_for(milliseconds(target - dt*2));
			
			now = steady_clock::now();
			dt = duration_cast<milliseconds>(now - start).count();

			avgElapse = avgElapse - (avgElapse / weight) + dt / weight;
			avgPackets=avgPackets - (avgPackets/ weight) +numPkt/ weight;

			fps = 1000 / avgElapse;

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
			ship.Draw(c, Vec2(20, 20), 1);
			//c.DrawPointLine(1, 1, c.width() - 1, c.height() - 1, Color::Red);
			//c.DrawBlock(0, 0, true, Color::Green);
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