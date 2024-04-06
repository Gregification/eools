#pragma once

#include "Server.hpp"

void Server::run(ScreenInteractive& screen) {
	gmap.makeGrid({ 0,1 });

	screenThread = std::thread([&]() {
			auto userPane = Renderer([&]() {
				std::lock_guard lk(renderMutex);

				Elements eles;
				{
					std::lock_guard lk(m_mutexDeqConnections);
					for (auto& sptr : m_deqConnections) {
						try {
							if (!sptr->isConnected()) continue;

							uint32_t id = sptr->GetID();

							eles.push_back(text(std::format("{0:3} RTping:{1:6} {2}", std::to_string(id), std::to_string(sptr->pingTime.load()), sptr->ToString())) | bold);
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
								if (sptr->isConnected() && sptr->GetID() == id) {
									sptr->Disconnect();
									return;
								}
						}, ButtonOption::Ascii()),
					Button("ban ip of user", [&] {
							if (usrSelected.size() == 0) return;
							int id = std::stoi(usrSelected);
							std::lock_guard lk(m_mutexDeqConnections);
							for (auto& sptr : m_deqConnections)
								if (sptr->isConnected() && sptr->GetID() == id) {
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

	Start();

	while (1) {
		if (!Update())
			Sleep(30);
	}
}

//returns true/accept or false/decline regarding the connection. think Swing predicate filters
bool Server::onClientConnect(std::shared_ptr<net::connection<NetMsgType>> client) {
	std::lock_guard lk(renderMutex);
	
	if (blacklist_ip.contains(client->GetIP()))
		return false;

	net::message<NetMsgType> msg;
	msg.header.id = NetMsgType::Ping;
	Ping ping = Ping();
	ping.tag();
	msg << ping;

	client->Send(msg);

	return true;
}

void Server::onClientDisconnect(std::shared_ptr<net::connection<NetMsgType>> client) {

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
	messages.push_back(text("grid gof idx:" + std::to_string(Grid::gof.index)));
}

//on message from specific given client
void Server::OnMessage(std::shared_ptr<net::connection<NetMsgType>> client, net::message<NetMsgType>& msg) {
	std::lock_guard lk(renderMutex);

	switch (msg.header.id) {
		case NetMsgType::Ping : {
				Ping ping = Ping();
				msg >> ping;

				if (ping.isComplete()) {
					client->pingTime = ping.getTime();
					break;
				}
				
				ping.tag();
				msg << ping;
				MessageClient(client, msg);
			} break;
		case NetMsgType::IDPartition: {
				msg.body.clear();

				IDPartition part = IDPartition();
					part.min = partitionCounter * STD_PARTITION_SIZE;
					part.max = part.min + STD_PARTITION_SIZE;
					part.nxt = part.min;

				msg << part;
				partitionCounter++;

				MessageClient(client, msg);
			} break;
		case NetMsgType::IDCorrection : {
				static struct IDPartition backupIDParition(0, STD_PARTITION_SIZE - 1, 0);
				
				IDCorrection corr = {};
				msg >> corr;

				corr.newId = backupIDParition.getNext();

				msg << corr;
				MessageAllClients(msg);
			} break;
		default: {
			//broadcast to all clients excluding source client
			MessageAllClients(msg, client);
		} break;
	}

	messages.push_back(text(std::format("[received message] ordinal: {}", static_cast<int>(msg.header.id))));
}
