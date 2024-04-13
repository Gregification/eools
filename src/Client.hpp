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
	//friend class Camera;
public:
	Client() : ship(std::make_shared<Ship>(LOCAL_PARITION.getNext())) {

	}

	~Client() = default;

	void run(ScreenInteractive& screen) override;

public:
	float fps = 0;
	id_t currentGrid;

protected:
	void OnMessage(net::message<NetMsgType> msg) override;

protected:
	//game
	GameMap gameMap;
	std::shared_ptr<Ship> ship;

private:
	bool gridIsReady = false;
	Vec2 mouse, camOffset;
	float scale = 1;

	Component Renderer_play();
	Component Renderer_map();
	Component Renderer_upgrades();
	Component Renderer_inventory();

	void onInput(Event e);

	void Draw(Canvas& c);
};