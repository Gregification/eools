#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

#include "Server.hpp"
#include "SceneManager.hpp"

bool logCommonEvents = false;

void Server::run(ScreenInteractive& screen) {
	//debug
	messageViewer->Post_Message("see NetMessageType.h for message types.");
	messageViewer->Post_Message("gobj cid:" + std::to_string(IdGen<GameObject>::gof.class_id));
	messageViewer->Post_Message("grid cid:" + std::to_string(IdGen<Grid>::gof.class_id));
	messageViewer->Post_Message("ship cid:" + std::to_string(IdGen<Ship>::gof.class_id));
	/*messageViewer->Post_Message("ship inst cid:" + std::to_string(Ship().GetClassId()));
	messageViewer->Post_Message("ship inst cid:" + std::to_string(Ship().GetClassId()));
	messageViewer->Post_Message("grid inst cid:" + std::to_string(Grid().GetClassId()));*/

	//TODO: change to use ftxui::Loop, idk y I went with a thread that day
	screenThread = std::thread([&]() {
			auto userPane = Renderer([&]() {
				std::lock_guard lk(renderMutex);

				Elements eles;
				{
					std::lock_guard lk(m_mutexDeqConnections);
					for (auto& sptr : m_deqConnections) {
						try {
							if (!sptr->isConnected()) continue;

							Instance_Id id = sptr->connectionID;

							eles.push_back(text(
									std::format("{:3} RTping:{:6} {}",
										std::to_string(id),
										std::to_string(sptr->pingTime),
										sptr->ToString())
								) | bold
							);
						}
						catch (std::exception& e) {
							onError(e.what());
						}
					}
				}

				return window(text(std::format("users (hosted on port:{0})", std::to_string(listeningPort))), vbox(std::move(eles)));
			});

			std::string usrSelected;
			Component usrInput = Input(&usrSelected);
			usrInput |= CatchEvent([&](Event event) {
					return event.is_character() && !std::isdigit(event.character()[0]) && usrSelected.size() > 260;
				});

			auto controlPane = Container::Vertical({
					usrInput | borderLight,
					Button("kick", [&] {
							if (usrSelected.size() == 0) return;
							int id = std::stoi(usrSelected);
							std::lock_guard lk(m_mutexDeqConnections);
							for (auto& sptr : m_deqConnections)
								if (sptr->isConnected() && sptr->connectionID == id) {
									sptr->Disconnect();
									return;
								}
						}, ButtonOption::Ascii()),
					Button("ban ip of user", [&] {
							if (usrSelected.size() == 0) return;
							int id = std::stoi(usrSelected);
							std::lock_guard lk(m_mutexDeqConnections);
							for (auto& sptr : m_deqConnections)
								if (sptr->isConnected() && sptr->connectionID == id) {
									blacklist_ip.insert(sptr->GetIP());
									return;
								}
						}, ButtonOption::Ascii()),
					Button("ban exact ip given", [&] {
							std::lock_guard lk(renderMutex);
							if (usrSelected.size() == 0) return;
							blacklist_ip.insert(usrSelected);
						}, ButtonOption::Ascii()),
					Button("unban exact ip given", [&] {
							std::lock_guard lk(renderMutex);
							if (usrSelected.size() == 0) return;
							blacklist_ip.erase(usrSelected);
						}, ButtonOption::Ascii()),
					Checkbox("log common messages", &logCommonEvents),
				});

			auto bannedPane = Renderer([&]() {
					std::lock_guard lk(renderMutex);

					Elements eles;
					for (auto& ipstr : blacklist_ip)
						eles.push_back(text(ipstr) | bold);

					return window(text("ip blacklist"), vbox(std::move(eles)) | frame | vscroll_indicator);
				});

			int ctrlPSizePref = 30;
			auto container = ResizableSplitLeft(controlPane, bannedPane, &ctrlPSizePref);
			int usrSizePref = 10;
			container = ResizableSplitBottom(container, userPane, &usrSizePref);
			int msgSizePref = 50;
			container = ResizableSplitLeft(messageViewer, container, &msgSizePref);

			screen.Loop(container);
		});

	primeGameMap();

	Start();

	//control loop
	{
		using namespace std::chrono;

		long long dt;
		const long long //milliseconds
			pingTarget		= 1000 * 2, 
			syncTarget		= 1000 / 2; 

		int pkts;

		for (;;) {
			time_point start = high_resolution_clock::now();

			pkts = Update();

			//huh https://www.nvidia.com/content/gtc/documents/1077_gtc09.pdf

			static auto lastPingTime = high_resolution_clock::now();
			if (duration_cast<milliseconds>(start - lastPingTime).count() > pingTarget) {
				lastPingTime = start;

				auto ping = Ping();
					ping.tag();

				net::message<NetMsgType> msg;
				msg.header.id = NetMsgType::Ping;

				msg << ping;

				MessageAllClients(msg);

				screen.Post(Event::Custom);
			}

			static auto lastSyncTime = high_resolution_clock::now();
			if (duration_cast<milliseconds>(start - lastSyncTime).count() > syncTarget) {
				lastSyncTime = start;
			}
		}
	}
}

//returns true/accept or false/decline regarding the connection. predicate filters
bool Server::onClientConnect(std::shared_ptr<net::connection<NetMsgType>> client) {
	std::lock_guard lk(renderMutex);
	
	if (blacklist_ip.contains(client->GetIP()))
		return false;

	connectionStatus.insert({client->connectionID, ConnectionStatus()});

	return true;
}

void Server::onClientDisconnect(std::shared_ptr<net::connection<NetMsgType>> client) {
	std::lock_guard lk(renderMutex);

	messageViewer->Post_Message((
		std::format("[EVENT][DISCONNECT] lost {}, {}",
			client->connectionID, 
			connectionStatus.find(client->connectionID)->second.isQueue ? "Q" : "C"
		)),
		ftxui::color(ftxui::Color::CyanLight));

	connectionStatus.erase(client->connectionID);
}

//for user purposes, good luck tring to use this for anyhting otherwise. rip
void Server::onError(std::string message) {
	std::lock_guard lk(renderMutex);
	messageViewer->Post_Message("[ERROR]" + message, ftxui::color(ftxui::Color::Red) | ftxui::underlined);
}

//on notable evets such as connection being denied, user joining and so on
void Server::onEvent(std::string message) {
	std::lock_guard lk(renderMutex);
	messageViewer->Post_Message("[EVENT]" + message, ftxui::color(ftxui::Color::GrayLight));
}

//on message from specific given client
void Server::OnMessage(std::shared_ptr<net::connection<NetMsgType>> client, net::message<NetMsgType>& msg) {
	std::lock_guard lk(renderMutex);

	switch (msg.header.id) {
		case NetMsgType::Ping:
			if (!logCommonEvents) break;

		case NetMsgType::GridRequest:
		case NetMsgType::GameObjectUpdate:

		//the ones that should alwayse be visible
		case NetMsgType::ConnectionStatus:
		case NetMsgType::IDCorrection:
		case NetMsgType::IDPartition:
		case NetMsgType::RequestById:
			messageViewer->Post_Message(
				std::format("[received] - {}", 
					BE_NetMsgType::_from_index(msg.header.id)._to_string()),
				ftxui::color(ftxui::Color::Yellow));
			break;

		default:;
	}

	switch (msg.header.id) {
		case NetMsgType::Ping : {
				Ping ping = Ping();
				msg >> ping;

				if (ping.isComplete()) {
					client->pingTime = ping.getTime();
				} else {
					client->pingTime = ping.tag();

					msg << ping;
					MessageClient(client, msg);
				}
				
			} break;
		case NetMsgType::IDPartition: {
				static int partitionCounter = 1; //0'th reserved for server
				
				msg.body.clear();

				IDPartition part = IDPartition();
					part.min = partitionCounter * STD_PARTITION_SIZE;
					part.max = part.min + STD_PARTITION_SIZE - 1;
					part.nxt = part.min;

				msg << part;
				partitionCounter++;

				MessageClient(client, msg);
			} break;
		case NetMsgType::IDCorrection : {
				static struct IDPartition backupIDParition(BAD_ID+1, STD_PARTITION_SIZE - 1, 0);

				auto corr = IDCorrection();
				msg >> corr;

				corr.newId = backupIDParition.getNext();

				msg << corr;
				MessageAllClients(msg);
			} break;
		case NetMsgType::GameObjectUpdate: {

				MessageAllClients(msg);
			} break;
		case NetMsgType::ConnectionStatus: {
				auto cs = ConnectionStatus();
				msg >> cs;

				connectionStatus.insert({ client->connectionID, cs });
			}break;
		case NetMsgType::GridRequest: {
				auto gr = GridRequest();
				msg >> gr;

				auto grid = SceneManager::GetGrid(gr.pos);
				
				msg.body.clear();
				msg.header.id = NetMsgType::GameObjectPost;

				//message packing
				grid->packMessage(msg);
				grid->GameObject::packMessage(msg);

				/*msg << IdGen<Grid>::gof.class_id;
				msg << IdGen<GameObject>::gof.class_id;
				msg << size_t(2);*/

				Classes clas({
					IdGen<Grid>::gof.class_id,
					IdGen<GameObject>::gof.class_id,
				});
				clas.Pack(msg);

				GameObjectPost package{
					.isGrid = true,
					.grid_id = grid->id(),
					.time = GetTime(),
				};
				msg << package;

				MessageClient(client, msg);
			}break;

		default: {}
	}

}

void Server::primeGameMap() {
	//gameMap.getGrid({ 0,0 });
}