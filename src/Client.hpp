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
	Client();
	~Client() = default;

	float refreshesPS = -1;

	inst_id currentGrid_id = 0;

	void run(ScreenInteractive& screen) override;

protected:
	GameMap gameMap;
	std::shared_ptr<Ship> ship;

	void OnMessage(net::message<NetMsgType> msg) override;

	Camera cam;
	Vec2 mouse;

private:
	bool gridIsReady = false;
	bool showNewWindowModal = false;
	float avgPackets = 0;

	void initControls();

	void OpenNewWindowDialogue();
	void OnMouse(Event e);

	Component mainContainer;
	Component windowContainer;
	Component clientStats;

	Component Renderer_play();
	Component Renderer_map();
	Component Renderer_upgrades();
	Component Renderer_inventory();	

	void Draw(Canvas& c);
	
};
