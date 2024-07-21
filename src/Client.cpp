#include "Client.hpp"

#include "Game/Interfaces/InterfaceContent.hpp"
#include <ftxui/component/component.hpp>
#include "Game/Interfaces/IFOptions.hpp"

Client::Client() : 
	ship(std::make_shared<Ship>(IDPartition::LOCAL_PARITION.getNext())),
	raw_mouse_screen(0) {
	//init ui
	{
		windowContainer = Container::Stacked({});

		clientStats = Renderer([&] {
			static Element buffer = text("pester your cat");
			static int i = 0;
			if (i++ % 50 == 0)//less visually distracting if its not jittering all the time. used a arbitrary #.
				buffer = text(std::format(" ~pkts:{:3.0f} | fps:{:3.0f} | ping:", avgPackets, refreshesPS)
					+ (m_connection->isConnected() ? std::to_string(m_connection->pingTime) : "LOST CONNECTION"));

			return buffer;
			});

		auto rend = Renderer_play() | flex;

		//init  modal
		Component modalContainer = Container::Stacked({});
		{
			Component list = Container::Vertical({});
			static auto list_states = std::array<bool, InterfaceContent::publicInterfaces.size()>();

			for (int i = 0; i < InterfaceContent::publicInterfaces.size(); i++) {
				auto name = std::to_string(i) + ": " + InterfaceContent::publicInterfaces[i].first;
				list->Add(Checkbox(
					name,
					&list_states[i]
				));
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

									auto sp = cont.second(*this);
									interfaceWindows.push_back(std::weak_ptr<InterfaceContent>(sp));

									windowContainer->Add(Window({
											.inner = sp,
											.title = cont.first,
											.left = i * 7 % 50,
											.top = i * 7 % 50,
										}));

									break;
								}
						}),
						Button("clear",		[&] { for (bool& v : list_states) v = false; }),
						Button("cancel",	[&] { SetNewWindowDialogue(false); })
					})
				}) | Renderer([&](Element content) {
					return vbox({
						text("select windows to (not) open together"),
						separator(),
						content
					});
				}) | borderHeavy | bgcolor(Color::Blue);

			modalContainer |= Modal(modal, &showNewWindowModal);
		}


		/*explination of cheesed solution for focoused window becaue this is really 
		*	starting to look like spaghetti code. all the code for this is in the 
		*	CatchEvent decorator of [windowContainer] and [rend]
		* 
		* problem: ftxui isnt able to correct the focus to what container was last used,
		*	and because of stacked container ordering, input ment for the canvas is 
		*	intercepted by the [windowContainer] first (very bad, cannot reorder stack).
		* 
		* solution: because mouse events are alwayse caught by the windows, use as 
		*	a indicator.
		*/


		auto windowContainer_wrapper = windowContainer | CatchEvent([&](Event e) {
			static bool wasWinSelected = false;

			if (isWindowSelected != wasWinSelected) {
				wasWinSelected = isWindowSelected;

				Events::ClientEvent::observer.invokeEvent<>(
					isWindowSelected ? 
						Events::ClientEvent::CLIENT_EVENT::ON_WINDOW_FOCUS 
					:	Events::ClientEvent::CLIENT_EVENT::ON_WINDOW_UNFOCUS
				);
			}

			if (!isWindowSelected && !showNewWindowModal) {
				if (e.is_mouse()) {
					OnMouse(std::move(e));

					if(e.mouse().button != Mouse::None)
						return true;
				}

				if (e.is_character()) {
					Events::KeyBinds::sendKey(std::move(e));
					return true;
				}
			}

			if (e.is_mouse() && e.mouse().button == Mouse::None)
				isWindowSelected = true;

			return false;
		});

		//put this with any new containers
		auto windowFocusDetect = CatchEvent([&](Event e) {
			if (e.is_mouse()) {
				if (e.mouse().button == Mouse::None)
					isWindowSelected = false;

				return true;
			}

			return false;
		});
		
		mainContainer = Container::Stacked({
				clientStats,
				modalContainer,
				windowContainer_wrapper,
				rend | windowFocusDetect,
			});
	}
}

std::shared_ptr<Ship> Client::getShip() const {
	return ship;
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

		unresolvedResponder.push_back([pos = gr.pos, this](Client&)->bool {
				auto g = SceneManager::GridAt(pos);
				if (g) {
					currentGrid = g.value();
					
					//add ship
					currentGrid->AddObject(ship);

					//send POST to add this player
					Send(SceneManager::POST(currentGrid->id(), ship.get()));

					//trigger the ship viewer thing
					Events::ClientEvent::observer.invokeEvent(
						Events::ClientEvent::CLIENT_EVENT::ON_SHIP_OPERABLE_SHIP_FOCOUS,
						ship
					);

					return true;
				}
				return false;
			});
	}

	//main
	//TODO: fix it better
	{
		using namespace std::chrono;

		Loop loop(&screen, mainContainer);

		time_point start = steady_clock::now();

		const time_t target = 1000 / 25;
		time_t dt;

		float numPkt, avgElapse = target;
		const float weight = 1.0f/10;

		while(!loop.HasQuitted()) {
			start = steady_clock::now();


			numPkt = Update();

			for (int i = 0; i < unresolvedResponder.size(); i++) {
				if (unresolvedResponder[i](*this)) {
					unresolvedResponder.erase(unresolvedResponder.begin() + i);
					i--;
				}
			}

			loop.RunOnce();

			if (currentGrid) {
				SceneManager::processGrid(
					currentGrid.get(),
					target,
					[&](Message m) { Send(std::move(m)); },
					[ship = ship.get()](GameObject* g) { return g == ship; }
				);
			}

			dt = duration_cast<milliseconds>(steady_clock::now() - start).count();

			if (false && dt < target) {
				std::this_thread::sleep_for(milliseconds(target - dt));
				dt = duration_cast<milliseconds>(steady_clock::now() - start).count();
			}

			avgElapse = avgElapse + (dt - avgElapse) * weight;
			avgPackets= avgPackets+ (numPkt - avgPackets) * weight;
			refreshesPS = 1000.0 / avgElapse + target * 0.1;//counter is a bit janky

			//screen.PostEvent(Event::Custom);
		}
	}
}

void Client::OnMessage(net::message<NetMsgType> msg) {
	switch (msg.header.id) {
		case NetMsgType::GridRequest:
		case NetMsgType::GameObjectPost:
		case NetMsgType::IDCorrection:
		case NetMsgType::ConnectionStatus:
		case NetMsgType::IDPartition:
		case NetMsgType::RequestById:
			Events::ClientEvent::observer.invokeEvent(
				Events::ClientEvent::CLIENT_EVENT::EVENT_MESSAGE,
				BE_NetMsgType::_from_index(msg.header.id)._to_string()
			);
		default:;
	}

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
				IDPartition idp;
				msg >> idp;
				if (!idp.IsBad())
					IDPartition::LOCAL_PARITION = idp;
			}break;
		case NetMsgType::ConnectionStatus: {
				auto stat = ConnectionStatus();
					stat.isQueue = false;

				msg._body.clear();
				msg << stat;
				Send(msg);
			}break;
		case NetMsgType::GridRequest: {
				assert(false);//debug
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
		cam.Draw(c, currentGrid);
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

		static float transX = 4, transY = 4, xs = 1, ys = 1;
		transX += o * xs;
		transY += o * ys;

		if(transX > w/l || transX < 3) xs *= -1;
		if(transY > h/l || transY < 3) ys *= -1;
		
		for (int i = 1; i < l; i++) {
			c.DrawPointEllipse(
				w2 + (raw_mouse_screen.x - w2) *i*i / w2,
				h2 + (raw_mouse_screen.y - h2) *i*i / h2,
				transX * i,
				transY * i
			);
		}
	}
}

void Client::SetNewWindowDialogue(bool c) {
	showNewWindowModal = c;
}

void Client::OnMouse(Event e) {
	assert(e.is_mouse());

	//i fogot why Client::raw_mouse_screen exists since cam::mouse_screen does the 
	//		exact same thing for the mose part
	Vec2_i dm(
		(e.mouse().x - 1) * 2 - raw_mouse_screen.x,
		(e.mouse().y - 1) * 4 - raw_mouse_screen.y
	);

	Vec2 mog_zoomRefrence;

	raw_mouse_screen += dm;
	cam.mouse_screen = raw_mouse_screen;
	cam.mouse_screen.x += 3;
	cam.mouse_screen.y += 4;

	switch (e.mouse().button) {
		case Mouse::Left: {
				//ship->transform.position = cam.screenToGrid(cam.mouse_screen);
			using namespace Events;

			ClientEvent::observer.invokeEvent<GameObjPtr>(
				ClientEvent::CLIENT_EVENT::ON_GAMEOBJECT_SELECT,
				currentGrid->ObjectAt(cam.screenToGrid(cam.mouse_screen))
			);

			}break;
		case Mouse::Middle: {

			cam.offX() += dm.x;
			cam.offY() += dm.y;

			}break;
		case Mouse::Right: {

			static std::weak_ptr<IFOptions> former;

			if (former.expired()) {
				const Vec2_i pos{ e.mouse().x - 5, e.mouse().y - 5};

				std::shared_ptr<IFOptions> ops = ftxui::Make<IFOptions>(pos, *this);
				former = ops;

				interfaceWindows.push_back(ops);

				windowContainer->Add(Window({
						.inner = ops,
						.left = e.mouse().x - 5,
						.top = e.mouse().y - 5,
					}));
			}

			}break;
		case Mouse::WheelUp: {
			mog_zoomRefrence = cam.screenToGrid(cam.mouse_screen);

			cam.trans.scaleX() = std::max(0.0f, cam.trans.scaleX() * 0.2f);
			cam.trans.scaleY() = std::max(0.0f, cam.trans.scaleY() * 0.2f);

			goto scaleChange;
			}break;
		case Mouse::WheelDown: {

			mog_zoomRefrence = cam.screenToGrid(cam.mouse_screen);

			cam.trans.scaleX() += cam.trans.scaleX() * 0.2f;
			cam.trans.scaleY() += cam.trans.scaleY() * 0.2f;

			goto scaleChange;
			}break;

		{
		scaleChange:
			//adjust cam to matching position
			auto mos = cam.gridToScreen(mog_zoomRefrence) - cam.mouse_screen;

			cam.offX() = -mos.x;
			cam.offY() = -mos.y;

			break;
		}

		default: break;
	}
}

template<typename T>
std::shared_ptr<Events::Listener<T>> Client::addListener(std::shared_ptr<Events::Listener<T>> l){
	listeners.push_back(l);
	return l;
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

	auto network_send_message = addListener(MakeListener<net::message<NetMsgType>>(
		[&](const net::message<NetMsgType>& m) { 
			Send(m); 
		}));

	/***********************************************************************************************************
	* audio
	***********************************************************************************************************/

	auto DEBUG_beep = addListener(MakeListener<>(
		[] {
			std::cout << "\a" << std::endl; 
		}));
	
	auto DEBUG_beep_string = addListener(MakeListener<std::string>(
		[](std::string) {
			std::cout << "\a" << std::endl; 
		}));

	/***********************************************************************************************************
	* client event
	***********************************************************************************************************/

	auto DEBUG_add_new_demo_window = addListener(MakeListener<>(
		[&] {
			static int a = 0;
			auto b = Window({ .title = "on call" + std::to_string(a++) });
			windowContainer->Add(b);
		}));

	auto DEBUG_message_event = addListener(MakeListener<>(
		[&] {
			static int a = 0;
			ClientEvent::observer.invokeEvent(ClientEvent::CLIENT_EVENT::EVENT_MESSAGE, std::to_string(a++));
		}));

	auto on_window_focous = addListener(MakeListener<>(
		[&] {
			for (int i = 0; i < interfaceWindows.size(); i++) {
				auto sp = interfaceWindows[i].lock();
				if (!sp) {
					interfaceWindows.erase(interfaceWindows.begin() + i);
					i--;
					continue;
				}

				sp->OnFocus();
			}
		}));

	auto on_window_unfocous = addListener(MakeListener<>(
		[&] {
			for (int i = 0; i < interfaceWindows.size(); i++) {
				auto sp = interfaceWindows[i].lock();
				if (!sp) {
					interfaceWindows.erase(interfaceWindows.begin() + i);
					i--;
					continue;
				}

				sp->OnUnfocus();
			}

			/* some interfaces may get detached but that dosent mean the window itself 
			* is detached, this goes through a and removes windows that are empty 
			*/
			unresolvedResponder.push_back([&](Client&) -> bool {
				for (int i = 0; i < windowContainer->ChildCount(); i++) {
					auto c = windowContainer->ChildAt(i);

					if (c->ChildCount() == 0) {
						c->Detach();
						i--;
					}
				}

				return true;
			 });
		}));

	auto open_new_window_dialogue = addListener(MakeListener(
		[&] {
			showNewWindowModal = !showNewWindowModal;
		}));

	auto add_new_window = addListener(MakeListener<std::function<Component(Client&)>>(
		[&] (auto f){
			Component comp = f(*this);
			windowContainer->Add(comp);
			comp->TakeFocus();
		}));

	auto delete_selected_window = addListener(MakeListener(
		[&] {
			if (windowContainer->ChildCount() > 0){
				auto ac = windowContainer->ActiveChild();
				if (ac)
					ac->Detach();
			}
		}));

	/***********************************************************************************************************
	* game
	***********************************************************************************************************/

	auto DEBUG_send_ship_update = addListener(MakeListener(
		[&] {
			Send( //just abuse POST as update
				SceneManager::POST(currentGrid->id(), ship.get())
			);
		}));

	/***********************************************************************************************************
	* bindings
	***********************************************************************************************************/

	//ClientEvent::observer.AddListenerToEvent(ClientEvent::CLIENT_EVENT::EVENT_MESSAGE, DEBUG_beep_string);
	//KeyBinds::observer.AddListenerToEvent(KeyBinds::CONTROL_EVENT::DISPLAY_NEW_WINDOW, DEBUG_add_new_demo_window);
	//KeyBinds::observer.AddListenerToEvent(KeyBinds::CONTROL_EVENT::DISPLAY_NEW_WINDOW, DEBUG_message_event);
	KeyBinds::observer.AddListenerToEvent(KeyBinds::CONTROL_EVENT::DISPLAY_NEW_WINDOW, open_new_window_dialogue);
	//KeyBinds::observer.AddListenerToEvent(KeyBinds::CONTROL_EVENT::DISPLAY_REMOVE_WINDOW, DEBUG_beep);
	KeyBinds::observer.AddListenerToEvent(KeyBinds::CONTROL_EVENT::DISPLAY_REMOVE_WINDOW, delete_selected_window);
	//KeyBinds::observer.AddListenerToEvent(KeyBinds::CONTROL_EVENT::DEBUG_btn, DEBUG_send_ship_update);
	
	ClientEvent::observer.AddListenerToEvent(
		ClientEvent::CLIENT_EVENT::ON_WINDOW_FOCUS,
		on_window_focous
	);
	ClientEvent::observer.AddListenerToEvent(
		ClientEvent::CLIENT_EVENT::ON_WINDOW_UNFOCUS, 
		on_window_unfocous
	);
	ClientEvent::observer.AddListenerToEvent(
		ClientEvent::CLIENT_EVENT::ADD_TO_WINDOW_CONTAINER, 
		add_new_window
	);

	//Network::observer.AddListenerToEvent(Network::NETWORK_EVENT::SEND_MESSAGE, network_send_message);

}
