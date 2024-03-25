#pragma once

#include "App.hpp"

#include "NetMessageType.h"
#include <eol_net.hpp>

class QueueClient : public App, public net::client_interface<NetMsgType> {
	friend class Client;

public:
	QueueClient() = default;

public:
	void run(ScreenInteractive& screen) override;

protected:
	void OnMessage(net::message<NetMsgType> msg) override;

private:
	void startGame();
	void promptConnection(ScreenInteractive& screen);

private:
	ftxui::Elements messages;
	std::atomic_bool isReady = false;
};