#include "Client.hpp"

#include "Game/Interfaces/InterfaceContent.hpp"
#include <ftxui/component/component.hpp>

Client::Client() : ship(std::make_shared<Ship>(LOCAL_PARITION.getNext())) {
	//init ui
	{
		windowContainer = Container::Stacked({});

		clientStats = Renderer([&] {
			static Element buffer = text("pester your cat");
			static int i = 0;
			if (i++ % 100 == 0)//less visually distracting if its not jittering all the time. used a arbitrary #.
				buffer = text(std::format(" ~pkts:{:3.0f} | rps:{:3.0f} | ping:", avgPackets, refreshesPS)
					+ (m_connection->isConnected() ? std::to_string(m_connection->pingTime) : "LOST CONNECTION"));

			return buffer;
			});

		auto rend = Renderer_play() | flex
			| CatchEvent([&](Event e) {
			if (e.is_mouse()) {
				OnMouse(std::move(e));
				return true;
			};
			return false;
				});

		//init  modal
		Component modalContainer = Container::Stacked({});
		{
			Component list = Container::Vertical({});
			auto list_states = std::array<bool, InterfaceContent::publicInterfaces.size()>();

			for (int i = 0; i < InterfaceContent::publicInterfaces.size(); i++) {
				list->Add(Checkbox(std::to_string(i) + ": " + InterfaceContent::publicInterfaces[i].first, &list_states[i]));//C28020, "by design" apparently
			}

			Component modal = Container::Vertical({
				list,
				Container::Horizontal({
						Button("confirm",	[&] { 
							SetNewWindowDialogue(false);
							
							//inst selected windows
							for(int i = 0; i < list_states.size(); i++)
								if (list_states[i]) {
									auto const &cont = InterfaceContent::publicInterfaces[i];

									windowContainer->Add(Window({
											.inner = cont.second(),
											.title = cont.first,
											.left = 0,
											.top = 1,
										}));

									break;
								}
						}),
						Button("clear",		[&] { for (bool& v : list_states) v = false; }),
						Button("cancel",	[&] { SetNewWindowDialogue(false);	for (bool& v : list_states) v = false; })
					})
				}) | Renderer([&](Element content) {
					return vbox({
						text("select windows to open together"),
						separator(),
						content
						});
				}) | borderHeavy | bgcolor(Color::Blue);

			modalContainer |= Modal(modal, &showNewWindowModal);
		}

		mainContainer = Container::Stacked({
				clientStats,
				modalContainer,
				windowContainer,
				rend,
			}) | CatchEvent([&](Event e) {
				if (e.is_mouse()) return false;
				Events::KeyBinds::sendKey(std::move(e));
				return true;
			});
	}
}

/*
	handles 
		- network connection.
		- user controlls.
		- ui
*/
void Client::run(ScreenInteractive& screen) {
	initEvents();

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
		GridRequest gr({ 0,0 });

		net::message<NetMsgType> msg;
		msg.header.id = NetMsgType::GridRequest;
		msg << gr;
		Send(msg);

		assert(!gr.pos.IsBad());
		unresolvedResponder.push_back([pos = gr.pos, & currGrid = currentGrid](Client&)->bool {
				auto g = SceneManager::GridAt(pos);
				if (g) {
					currGrid = g.value();
					return true;
				}
				return false;
			});
	}

	//main
	//TODO: fix it :(
	{
		using namespace std::chrono;

		Loop loop(&screen, mainContainer);

		time_point
			start	= steady_clock::now(),
			now		= start,
			lastPhysTime = start;

		const time_t target = 1000 / 30;
		time_t dt;

		float avgElapse = target;
		const float weight = 1.0f/10;

		for(;;) {		
			start = high_resolution_clock::now();

			if (!loop.HasQuitted())
				loop.RunOnce();

			float numPkt = Update();

			for (int i = 0; i < unresolvedResponder.size(); i++) {
				if (unresolvedResponder[i](*this)) {
					unresolvedResponder.erase(unresolvedResponder.begin() + i);
					i--;
				}
			}

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
	if(msg.header.id != NetMsgType::Ping)
		Events::ClientEvent::observer.invokeEvent(Events::ClientEvent::CLIENT_EVENT::EVENT_MESSAGE, "received: " + std::to_string(static_cast<int>(msg.header.id)));

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
				if (!idp.IsBad())
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
				
			} break;
		case NetMsgType::GameObjectUpdate: {
				GameObjectUpdate gou;
				msg >> gou;
				auto res = SceneManager::processMessage(msg, gou);
				if (res) Send(res.value());
			}break;
		case NetMsgType::GameObjectPost: {
				GameObjectPost gop;
				msg >> gop;
				auto res = SceneManager::processMessage(msg, gop);
				if (res) Send(res.value());
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
	if (currentGrid){
		Transformation_2D trans;
		currentGrid->Draw(c, trans);
		c.DrawText(10,10, "drawing current grid");
	} else {
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

void Client::SetNewWindowDialogue(bool c) {
	showNewWindowModal = c;
}

void Client::OnMouse(Event e) {
	if (e.is_mouse()) {
		Vec2 dm(0,0);
			dm.x = (e.mouse().x - 1) * 2 - mouse.x;
			dm.y = (e.mouse().y - 1) * 4 - mouse.y;
		mouse += dm;

		switch (e.mouse().button) {
			case Mouse::Left:
				break;
			case Mouse::Middle:
				break;
			case Mouse::Right:
					cam.offset += dm;
				break;
			case Mouse::WheelUp:
					cam.scale += 1 + cam.scale * 1.2;
				break;
			case Mouse::WheelDown:
					cam.scale -= 1 + cam.scale * 1.2;
				break;
		}
	}
}

void Client::initEvents() {
	using namespace Events;
	/***********************************************************************************************************
	* rules
	* - no inline Listeners (for readability)
	* - idc length of the lambda names, make it intutively descriptive
	* - under_score_naming_scheme
	* - debugging funcitons must have a name that starts with "DEBUG_" 
	* - must push to the listeners object because observer uses weak pointers
	***********************************************************************************************************/

	/***********************************************************************************************************
	* network
	***********************************************************************************************************/

	auto network_send_message = MakeListener<net::message<NetMsgType>>([&](const net::message<NetMsgType>& m) { Send(m); });
	listeners.push_back(network_send_message);

	/***********************************************************************************************************
	* audio
	***********************************************************************************************************/

	auto DEBUG_beep = MakeListener<>([] { std::cout << "\a" << std::endl; });
	listeners.push_back(DEBUG_beep);
	auto DEBUG_beep_string = MakeListener<std::string>([](std::string) { std::cout << "\a" << std::endl; });
	listeners.push_back(DEBUG_beep_string);

	/***********************************************************************************************************
	* client event
	***********************************************************************************************************/

	auto DEBUG_add_new_demo_window = MakeListener<>([&] {
			static int a = 0;
			auto b = Window({ .title = "on call" + std::to_string(a++) });
			windowContainer->Add(b);
		});
	listeners.push_back(DEBUG_add_new_demo_window);

	auto DEBUG_message_event = MakeListener<>([&]() {
			static int a = 0;
			ClientEvent::observer.invokeEvent(ClientEvent::CLIENT_EVENT::EVENT_MESSAGE, std::to_string(a++));
		});
	listeners.push_back(DEBUG_message_event);

	auto open_new_window_dialogue = MakeListener([&] {
			showNewWindowModal = !showNewWindowModal;
		});
	listeners.push_back(open_new_window_dialogue);

	auto delete_selected_window = MakeListener([&] {
			std::string message;

			int count = windowContainer->ChildCount();

			if (windowContainer->ChildCount() == 0) {
				message = "no children of window";
			} else {
				auto ac = windowContainer->ActiveChild();
				if (ac) {
					message = "detached a window!";
					ac->Detach();
				}
				else {
					message = "no window selected! :(";
				}
			}

			ClientEvent::observer.invokeEvent(ClientEvent::CLIENT_EVENT::EVENT_MESSAGE, message);
		});
	listeners.push_back(delete_selected_window);

	/***********************************************************************************************************
	* game
	***********************************************************************************************************/



	/***********************************************************************************************************
	* bindings
	***********************************************************************************************************/

	//ClientEvent::observer.AddListenerToEvent(ClientEvent::CLIENT_EVENT::EVENT_MESSAGE, DEBUG_beep_string);
	//KeyBinds::observer.AddListenerToEvent(KeyBinds::CONTROL_EVENT::DISPLAY_NEW_WINDOW, DEBUG_add_new_demo_window);
	//KeyBinds::observer.AddListenerToEvent(KeyBinds::CONTROL_EVENT::DISPLAY_NEW_WINDOW, DEBUG_message_event);
	KeyBinds::observer.AddListenerToEvent(KeyBinds::CONTROL_EVENT::DISPLAY_NEW_WINDOW, open_new_window_dialogue);
	//KeyBinds::observer.AddListenerToEvent(KeyBinds::CONTROL_EVENT::DISPLAY_REMOVE_WINDOW, DEBUG_beep);
	KeyBinds::observer.AddListenerToEvent(KeyBinds::CONTROL_EVENT::DISPLAY_REMOVE_WINDOW, delete_selected_window);

	//Network::observer.AddListenerToEvent(Network::NETWORK_EVENT::SEND_MESSAGE, network_send_message);

}
