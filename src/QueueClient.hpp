#pragma once

#include "App.hpp"
#include "Client.hpp"

#include "NetMessageType.h"
#include "NetCommon/eol_net.hpp"

class QueueClient : public App, public net::client_interface<NetMsgType> {
	friend class Client;

public:
	QueueClient() = default;


public:
	void run(ScreenInteractive& screen) override;

protected:
	void OnMessage(net::message<NetMsgType> msg) override;

	std::string 
		strip = "127.0.0.1",
		strport = std::to_string(SERVER_PORT);

private:
	void startGame(ScreenInteractive& screen);
	void promptConnection(ScreenInteractive& screen);

	ftxui::Elements messages;
	std::atomic_bool isReady = false;
};