#pragma once

#include "App.hpp"

#include "NetMessageType.h"
#include <eol_net.hpp>

class QueueClient : public App, public net::client_interface<NetMsgType> {
	friend class Client;

public:
	QueueClient() {}

	void run(ScreenInteractive& screen) override;

private:
	void startGame();
	void promptConnection(ScreenInteractive& screen);
};