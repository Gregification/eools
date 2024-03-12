#pragma once

#include "Server.hpp"
#include "ftxui/dom/canvas.hpp"

void Server::run(ScreenInteractive& screen) {
	screenThread = std::thread([&]() {
			auto userPane = Renderer([&]() {
				std::lock_guard lk(renderMutex);
				Elements eles;
				for (size_t i = 0; i < m_deqConnections.size(); i++) {
					uint32_t id = m_deqConnections.at(i)->GetID();

					eles.push_back(text(std::to_string(id)));
				}

				return window(text("users"), vbox(std::move(eles))) | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 50);
			});

			auto messagePane = Renderer([&]() {
				std::lock_guard lk(renderMutex);
				return window(text("messages"), vbox(messages)) | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 50);
			});

			int preffSize = 50;
			auto container = ResizableSplitLeft(messagePane, userPane, &preffSize);

			screen.Loop(container);
		});

	Start();

	bool didWork = false;//for readability
	while (1) {
		didWork = Update();

		//cap at 30 fps if nothing is happening
		if (!didWork)
			Sleep(33);
	}
}

//returns true/accept or false/decline regarding the connection. think Swing predicate filters
bool Server::onClientConnect(std::shared_ptr<net::connection<NetMsgType>> client) {
	return true;
}

void Server::onClientDisconnect(std::shared_ptr<net::connection<NetMsgType>> client) {

}

//on message from specific given client
void Server::OnMessage(std::shared_ptr<net::connection<NetMsgType>> client, net::message<NetMsgType>& msg) {
	std::lock_guard lk(renderMutex);

	messages.push_back(text("[MESSAGE]\n"));
}

//for user purposes, good luck tring to use this for anyhting otherwise. rip
void Server::onError(std::string message) {
	std::lock_guard lk(renderMutex);

	messages.push_back(text("[ERROR]" + message + "\n"));
	std::cout << "\a";
}

//on notable evets such as connection being denied, user joining and so on
void Server::onEvent(std::string message) {
	std::lock_guard lk(renderMutex);

	messages.push_back(text("[EVENT]" + message + "\n"));
}
