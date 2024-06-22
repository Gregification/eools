#include "Client.hpp"

#include "Game/Interfaces/IFInspector.hpp"

// naming conflict with a window api macro and ftxui canvas draw text method
#pragma push_macro("DrawText")
#undef DrawText

/*
	handles 
		- network connection.
		- user controlls.
*/
void Client::run(ScreenInteractive& screen) {
	
	//update connection status
	{
		auto stat = ConnectionStatus();
		stat.isQueue = false;

		net::message<NetMsgType> msg;
		msg.header.id = NetMsgType::ConnectionStatus;
		msg << stat;
		Send(msg);
	}

	//i'm lost, send me to a valid grid
	{
		auto gr = GridRequest();

		net::message<NetMsgType> msg;
		msg.header.id = NetMsgType::GridRequest;
		msg << gr;
		Send(msg);
	}

	//ui rendering
	float avgPackets = 0;
	auto clientStats = Renderer([&] {
		return text(std::format("~pkts:{:3.0f} | rps:{:3.0f} | ping:", avgPackets, refreshesPS)
			+ (m_connection->isConnected() ? std::to_string(m_connection->pingTime) : "LOST CONNECTION")
			+ " |");
		});

	main_container = Container::Stacked({
			clientStats,
			Container::Stacked({ Window({.title = "inited with"}) }),
			Renderer_play() | flex
		});

	auto winder = main_container->ChildAt(1);
	winder->Add(Window({ .title = "before loop" }));

	main_container |= CatchEvent([&](Event e) {

			if (e.is_character()) {
				KeyBinds::sendEvent(std::move(e));
				return true;
			}

			return false;
		});

	Loop loop(&screen, main_container);

	KeyBinds::ControlCall noise_cc = [](Event) { std::cout << "\a" << std::endl; };
	KeyBinds::ControlCall newWindowDialig = [&](Event) {			
			static int a = 0; 
			auto b = Window({ .title = "on call" + std::to_string(a++)});
			winder->Add(b);
		};

	//KeyBinds::SubToCtrlEvnt(KeyBinds::CONTROL_EVENT::DISPLAY_NEW_WINDOW, noise_cc);
	KeyBinds::SubToCtrlEvnt(KeyBinds::CONTROL_EVENT::DISPLAY_NEW_WINDOW, newWindowDialig);

	//main
	//TODO:bruh, this is ccavrearted. fix it :(
	{
		using namespace std::chrono;

		time_point
			start	= steady_clock::now(),
			now		= start,
			lastPhysTime = start;

		const long long target = 1000 / 30;
		long long dt;

		float avgElapse = target;
		const float weight = 1.0f/10;

		for(;;) {		
			start = high_resolution_clock::now();

			if (!loop.HasQuitted())
				loop.RunOnce();

			float numPkt = Update();

			dt = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();

			//if (dt < target) std::this_thread::sleep_for(milliseconds(target - dt));

			dt = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();

			avgElapse = avgElapse + (dt - avgElapse) * weight;
			avgPackets= avgPackets+ (numPkt - avgPackets) * weight;
			refreshesPS = 1000.0 / avgElapse + target * 0.1;//counter is a bit janky

			screen.PostEvent(Event::Custom);
		}
	}
}

void Client::OnMessage(net::message<NetMsgType> msg) {
	switch (msg.header.id) {
		case NetMsgType::Ping: {
				auto ping = Ping();
				msg >> ping;

				if (ping.isComplete()) {
					m_connection->pingTime = ping.getTime();
				} else {
					m_connection->pingTime = ping.tag();

					msg << ping;
					Send(msg);
				}

			}break;
		case NetMsgType::IDPartition: {
				IDPartition idp = IDPartition();
				msg >> idp;
				if (!idp.isBad())
					LOCAL_PARITION = idp;
			}break;
		case NetMsgType::ConnectionStatus: {
				auto stat = ConnectionStatus();
					stat.isQueue = false;

				msg.body.clear();
				msg << stat;
				Send(msg);
			}break;
		case NetMsgType::GridRequest: {
				auto gr = GridRequest();
				msg >> gr;
				

			} break;
		case NetMsgType::GameObjectUpdate: {
			gameMap.processMessage(
				msg, 
				[&](const net::message<NetMsgType>& m) -> void { Send(m); }
			);
			}break;
	}
}

Component Client::Renderer_play() {
	return Renderer([&] {
		return canvas([&](Canvas& c) {
			Draw(c);
		});
	});
		//Container::Stacked({Window({.title = "renderer_play"})})
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

void Client::Draw(Canvas& c) {//play renderer
	if (gridIsReady)
		gameCam.Draw(c, std::move(gameMap.getGrid(currentGrid_id)));
	else {
		c.DrawText(5,5, "play renderer, grid is not yet ready");
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
		Vec2 dm(0,0);
			dm.x = (e.mouse().x - 1) * 2 - mouse.x;
			dm.y = (e.mouse().y - 1) * 4 - mouse.y;
		mouse += dm;

		bool isGameControl = false;
		Camera& cam = client_tab == CLIENT_TAB::MAP ? mapCam : gameCam;
		switch (client_tab) {
			case CLIENT_TAB::CONTROL:
			case CLIENT_TAB::MAP: 
				isGameControl = true;
		}

		switch (e.mouse().button) {
			case Mouse::Left:
				break;
			case Mouse::Middle:
				break;
			case Mouse::Right:
				if(isGameControl) 
					cam.offset += dm;
				break;
			case Mouse::WheelUp:
				if (isGameControl)
					cam.scale += 1 + cam.scale * 1.2;
				break;
			case Mouse::WheelDown:
				if (isGameControl)
					cam.scale -= 1 + cam.scale * 1.2;
				break;
		}
	}
}

#pragma pop_macro("DrawText")