#pragma once

#include "QueueClient.hpp"

void QueueClient::run(ScreenInteractive& screen) {
	promptConnection(screen);
	while (!isConnected()) {
		std::cout << "\a";
		promptConnection(screen);
	}

	messages.push_back(text("waiting for server to assign id partition ..."));

	//request a id partition
	//too critical to do anything without
	{
		net::message<NetMsgType> msg;

		msg.header.id = NetMsgType::IDPartition;
		Send(msg);
	}

	//using screen thread as wait
	auto epicwaitingscreen = Renderer([&]() {
		if (isReady) {
			screen.Exit();
			return text("yippie");
		}

		std::lock_guard lk(renderMutex);
		return vbox(messages);
	});

	screen.Loop(epicwaitingscreen);

	startGame();
}

void QueueClient::startGame() {
	std::cout << "\a";
	Sleep(5000);
}

void QueueClient::OnMessage(net::message<NetMsgType>& msg) {
	switch (msg.header.id) {
		case NetMsgType::IDPartition: {
			msg >> LOCAL_PARITION;
			isReady = true;
			}break;
	}

	std::lock_guard lk(renderMutex);
	messages.push_back(text(std::to_string(static_cast<int>(msg.header.id)) + " \tsize:" + std::to_string(msg.size())));
}

void QueueClient::promptConnection(ScreenInteractive& screen) {
	static std::string strip = "127.0.0.1", strport = std::to_string(SERVER_PORT);

	Component i_ip = Input(&strip);
	i_ip |= CatchEvent([&](Event event) {
		return
			event.is_character()
			&& (!std::isdigit(event.character()[0]) || event.character()[0] == '.')
			&& strip.size() > 40;
		});

	Component i_port = Input(&strport);
	i_port |= CatchEvent([&](Event event) {
		return
			event.is_character()
			&& !std::isdigit(event.character()[0])
			&& strip.size() > 6;
		});

	auto confirmBtn = Button("go", [&] { screen.Exit(); }, ButtonOption::Ascii());

	auto comps = Container::Vertical({
		i_ip,
		i_port,
		confirmBtn
	});

	auto R = Renderer(comps, [&] {
		return vbox({
				hbox(text("ip	: "), i_ip->Render()) | size(WIDTH, GREATER_THAN, 50),
				hbox(text("port	: "), i_port->Render()) | size(WIDTH, GREATER_THAN, 50),
				confirmBtn->Render() | center
			}) | center | borderDashed;
	});

	screen.Loop(R);
	
	if (strip.length() == 0 || strport.length() == 0)
		return;
	
	uint16_t port = std::stoi(strport);

	Connect(strip, port);
}