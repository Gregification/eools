#pragma once

#include "NetCommon/eol_net.hpp"

#include "App.hpp"
#include "NetMessageType.hpp"

class Client : public App, public net::client_interface<NetMsgType> {
public:
	Client() {

	}

	~Client() = default;

public:
	void run(ScreenInteractive& screen) override;

protected:
	void OnMessage(net::message<NetMsgType> msg) override;

};