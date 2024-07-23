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

class InterfaceContent;

class Client : public App, public net::client_interface<NetMsgType> {
	friend class Camera;
	friend class IFOptions;

public:
	Client();
	~Client() = default;

	float refreshesPS = -1;

	void run(ScreenInteractive& screen) override;

	std::shared_ptr<Ship> getShip() const;

	/**
	* removes windows without content
	*/
	void removeEmptyWindows();

	Camera cam;

protected:
	std::shared_ptr<Ship> ship;
	std::shared_ptr<Grid> currentGrid;

	void OnMessage(net::message<NetMsgType> msg) override;

	Vec2_i raw_mouse_screen;

private:
	bool isWindowSelected = false;
	bool showNewWindowModal = false;
	float avgPackets = 0;
	
	//to keep client listeners alive since observer dosent handle it
	std::vector<std::shared_ptr<Events::ListenerBase>> listeners;
	
	//all open interfaces, seems like a bette rway to do things than
	//	dynamic casting
	std::vector<std::weak_ptr<InterfaceContent>> interfaceWindows;

	Component mainContainer;
	Component windowContainer;
	Component clientStats;

	/*
	* functions here are called every update, return true if it can be removed.
	* doubles as a nonblocking way to handle network requests, think ajax but even stupider.
	*/
	std::vector<std::function<bool(Client&)>> unresolvedResponder;

	void initEvents();

	void SetNewWindowDialogue(bool);
	void OnMouse(Event e);

	/*convience funciton for adding listeners, to reduce boiler plate*/
	template<typename T>
	std::shared_ptr<Events::Listener<T>> addListener(std::shared_ptr<Events::Listener<T>>);

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
