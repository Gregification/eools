#pragma once

#include "Server.hpp"

void Server::run(ScreenInteractive& screen) {
	//debug
	messages.push_back(text("grid gof id:" + std::to_string(Grid::staticGetGOF().class_id)));
	messages.push_back(text("ship gof id:" + std::to_string(Ship::staticGetGOF().class_id)));

	screenThread = std::thread([&]() {
			auto userPane = Renderer([&]() {
				std::lock_guard lk(renderMutex);

				Elements eles;
				{
					std::lock_guard lk(m_mutexDeqConnections);
					for (auto& sptr : m_deqConnections) {
						try {
							if (!sptr->isConnected()) continue;

							id_t id = sptr->connectionID;

							eles.push_back(text(
									std::format("{:3} RTping:{:6} {}",
										std::to_string(id),
										std::to_string(sptr->pingTime.load()),
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

			auto messagePane = Renderer([&]() {
				std::lock_guard lk(renderMutex);
				return window(text("messages"), vbox(messages));
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
					Button("clear messages", [&] {
							std::lock_guard lk(renderMutex);
							messages.clear();
						}, ButtonOption::Ascii()),
					
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
			//auto container = ResizableSplitBottom(controlPane, userPane, &usrSizePref);
			int msgSizePref = 50;
			container = ResizableSplitLeft(messagePane, container, &msgSizePref);

			screen.Loop(container);
		});

	primeGameMap();

	Start();

	for (size_t n = 0;; n = Update()) {

		if (n == 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(50));

		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			screen.Post(Event::Custom);

			using namespace std::chrono;
			
			static time_point 
				start = steady_clock::now(),
				now = start;
		
			now = steady_clock::now();
			long long dt = duration_cast<milliseconds>(now - start).count();

			if (dt > 2000) {
				start = now;
				Ping ping = Ping();
				ping.tag();

				net::message<NetMsgType> msg;
				msg.header.id = NetMsgType::Ping;
				msg << ping;

				MessageAllClients(msg);
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

	messages.push_back(text(
		std::format("[EVENT][DISCONNECT] lost {}, {}",
			client->connectionID, 
			connectionStatus.find(client->connectionID)->second.isQueue ? "Q" : "C"
		)));

	connectionStatus.erase(client->connectionID);
}

//for user purposes, good luck tring to use this for anyhting otherwise. rip
void Server::onError(std::string message) {
	std::lock_guard lk(renderMutex);
	messages.push_back(text("[ERROR]" + message + "\n"));
}

//on notable evets such as connection being denied, user joining and so on
void Server::onEvent(std::string message) {
	std::lock_guard lk(renderMutex);
	messages.push_back(text("[EVENT]" + message + "\n") | blink);
}

//on message from specific given client
void Server::OnMessage(std::shared_ptr<net::connection<NetMsgType>> client, net::message<NetMsgType>& msg) {
	std::lock_guard lk(renderMutex);

	messages.push_back(text(std::format("[received message] ordinal: {}", static_cast<int>(msg.header.id))));

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
				gameMap.processMessage(
					msg,
					[&](const net::message<NetMsgType>& m) -> void { MessageAllClients(m, client); }
				);
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

				std::shared_ptr<Grid> grid;
				if (gr.pos.isBad()) {
					//get random grid

				} else 
					grid = gameMap.getGrid(gr.pos);
				
				msg.body.clear();
				msg.header.id = NetMsgType::GameObjectUpdate;

			}break;

		default: {}
	}

}

void Server::primeGameMap() {
	gameMap.getGrid({ 0,0 });
}