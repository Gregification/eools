#pragma once

#include <format>
#include <chrono>
#include <ftxui/component/loop.hpp>

#include "NetCommon/eol_net.hpp"
#include "App.hpp"
#include "NetMessageType.hpp"
#include "GameObjects/Ship.hpp"
#include "Camera.hpp"
#include "Game/Events/Events.hpp"
#include "better-enums/enum.h"
#include "SceneManager.hpp"

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

	Instance_Id currentGrid_id = 0;

	void run(ScreenInteractive& screen) override;

protected:
	std::shared_ptr<Ship> ship;

	void OnMessage(net::message<NetMsgType> msg) override;

	Camera cam;
	Vec2 mouse;

private:
	bool gridIsReady = false;
	bool showNewWindowModal = false;
	float avgPackets = 0;
	std::vector<std::shared_ptr<Events::ListenerBase>> listeners;//purpose is to hold a active refrence for client listeners, the observer dosent handle it

	void initEvents();

	void SetNewWindowDialogue(bool);
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
