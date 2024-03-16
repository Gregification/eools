#pragma once

#include "QueueClient.hpp"

void QueueClient::run(ScreenInteractive& screen) {
	promptConnection(screen);
	while (!isConnected()) {
		std::cout << "\a";
		promptConnection(screen);
	}

	//ping for starter packet and id partition
	{
		net::message<NetMsgType> msg;
		msg.header.id = NetMsgType::StarterPacket;
		Send(msg);

		msg.header.id = NetMsgType::IDPartition;
		Send(msg);
	}

	//wait for starter packet and partition
	/*auto epicWaitingScreen = Renderer([] {
		
	});*/


	//yippie!
	startGame();
}

void QueueClient::startGame() {

}

void QueueClient::OnMessage(net::message<NetMsgType>& msg) {
	
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