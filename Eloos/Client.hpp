#pragma once

#include "App.hpp"

#include "NetMessageType.h"
#include <eol_net.hpp>

class Client : public App, public net::client_interface<NetMsgType> {
public:
	Client() {}

	void run(ScreenInteractive& screen);
};