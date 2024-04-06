#pragma once

#include <format>
#include <chrono>
#include <ftxui/component/loop.hpp>

#include "NetCommon/eol_net.hpp"
#include "App.hpp"
#include "NetMessageType.hpp"
#include "GameMap.hpp"

class Client : public App, public net::client_interface<NetMsgType> {
public:
	Client() {

	}

	~Client() = default;

public:
	float fps = 0;

	void run(ScreenInteractive& screen) override;

protected:
	GameMap gameMap;
	
	void OnMessage(net::message<NetMsgType> msg) override;

private:
	Component Renderer_play();
	Component Renderer_upgrades();
	Component Renderer_inventory();
};