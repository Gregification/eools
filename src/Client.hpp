#pragma once

#include <format>
#include <chrono>
#include <ftxui/component/loop.hpp>

#include "NetCommon/eol_net.hpp"
#include "App.hpp"
#include "NetMessageType.hpp"
#include "GameMap.hpp"
#include "GameObjects/Ship.hpp"
#include "Camera.hpp"
#include "Game/KeyBinds.hpp"
#include "better-enums/enum.h"

BETTER_ENUM(CLIENT_TAB, int,
	CONTROL,
	MAP,
	CARGO,
	EXPANSIONS
);

class Client : public App, public net::client_interface<NetMsgType> {
	friend class Camera;

public:
	Client() : ship(std::make_shared<Ship>(LOCAL_PARITION.getNext())){

	}

	~Client() = default;

	float fps = -1;

	inst_id currentGrid_id = 0;


	void run(ScreenInteractive& screen) override;

protected:
	GameMap gameMap;
	std::shared_ptr<Ship> ship;

	void OnMessage(net::message<NetMsgType> msg) override;

private:
	bool gridIsReady = false;

	Vec2 mouse;

	Camera gameCam, mapCam;

	Component Renderer_play();
	Component Renderer_map();
	Component Renderer_upgrades();
	Component Renderer_inventory();
	int client_tab = CLIENT_TAB::CONTROL;

	void onInput(Event e);

	void Draw(Canvas& c);
	
};
