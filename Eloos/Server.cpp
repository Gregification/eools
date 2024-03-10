#pragma once

#include "Server.hpp"
#include "ftxui/dom/canvas.hpp"                    // for Canvas

void Server::run(ScreenInteractive& screen) {
	screenThread = std::thread([&]() {
			auto R = Renderer([&]() {
				Elements children;
				for (size_t i = 0; i < messages.size(); ++i)
					children.push_back(text(messages[i]));
				return window(text("messages"), vbox(std::move(children)));
				});

			screen.Loop(R);
		});

	Start();

	while (1) {
		Update();
	}
}

//returns true/accept or false/decline regarding the connection. think Swing predicate filters
bool Server::onClientConnect(std::shared_ptr<net::connection<NetMessageType>> client) {
	return true;
}

void Server::onClientDisconnect(std::shared_ptr<net::connection<NetMessageType>> client) {

}

//on message from specific given client
void Server::OnMessage(std::shared_ptr<net::connection<NetMessageType>> client, net::message<NetMessageType>& msg) {
	messages.push_back("[MESSAGE]\n");
}

//for user purposes, good luck tring to use this for anyhting otherwise. rip
void Server::onError(std::string message) {
	messages.push_back("[ERROR]" + message + "\n");
}

//on notable evets such as connection being denied, user joining and so on
void Server::onEvent(std::string message) {
	messages.push_back("[EVENT]" + message + "\n");
	std::cout << "\a";
}
