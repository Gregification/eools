#pragma once

#include <format>
#include <chrono>
#include <ftxui/component/loop.hpp>

#include "NetCommon/eol_net.hpp"
#include "App.hpp"
#include "NetMessageType.hpp"
#include "GameMap.hpp"
#include "Game/Ship.hpp"

class Client : public App, public net::client_interface<NetMsgType> {
public:
	Client() : ship(LOCAL_PARITION.getNext()){

	}

	~Client() = default;

public:
	void run(ScreenInteractive& screen) override;

public:
	float fps = 0;
	Grid currentGrid;

protected:
	void OnMessage(net::message<NetMsgType> msg) override;

protected:
	GameMap gameMap;
	Ship ship;

private:
	Vec2 mouse;

	Component Renderer_play();
	Component Renderer_map();
	Component Renderer_upgrades();
	Component Renderer_inventory();
};