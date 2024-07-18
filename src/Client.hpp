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

class Client : public App, public net::client_interface<NetMsgType> {
	friend class Camera;

public:
	Client();
	~Client() = default;

	float refreshesPS = -1;

	void run(ScreenInteractive& screen) override;

	std::shared_ptr<Ship> getShip() const;

	Camera cam;

protected:
	std::shared_ptr<Ship> ship;
	std::shared_ptr<Grid> currentGrid;

	void OnMessage(net::message<NetMsgType> msg) override;

	Vec2_i mouse_screen;

private:
	bool isWindowSelected = false;
	bool showNewWindowModal = false;
	float avgPackets = 0;
	std::vector<std::shared_ptr<Events::ListenerBase>> listeners;//purpose is to hold a active refrence for client listeners, the observer dosent handle it
	
	/*
	* functions here are called every update, return true if it can be removed.
	* doubles as a nonblocking way to handle network requests, think ajax but even stupider.
	*/
	std::vector<std::function<bool(Client&)>> unresolvedResponder;

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
public:
	/****************************************************************
	* game controlls
	*  > most of these already have a corrosponding event and so 
	* dont actually need to be a defined method. done so anyways
	* incase I need access for something later 
	****************************************************************/

	void GoToGrid(Vec2 mapPos, Vec2 gridPos) {

	}
};
