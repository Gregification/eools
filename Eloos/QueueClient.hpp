#pragma once

#include "App.hpp"

#include "NetMessageType.h"
#include <eol_net.hpp>

class QueueClient : public App, public net::client_interface<NetMsgType> {
	friend class Client;

public:
	QueueClient() : loadingStatus(0) {}

public:
	void run(ScreenInteractive& screen) override;

protected:
	void OnMessage(net::message<NetMsgType>& msg) override;


private:
	void startGame();
	void promptConnection(ScreenInteractive& screen);

private:
	std::atomic<float> loadingStatus;

	ftxui::Elements messages;
	StarterPacket starterPacket = StarterPacket();
	std::atomic_bool isReady = false;
};