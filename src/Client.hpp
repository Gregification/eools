#pragma once

#include <format>
#include <chrono>
#include <ftxui/component/loop.hpp>

#include "App.hpp"
#include "better-enums/enum.h"
#include "Camera.hpp"
#include "Game/Events/Events.hpp"
#include "Game/InputControl.hpp"
#include "GameObjects/Ship.hpp"
#include "NetCommon/eol_net.hpp"
#include "NetMessageType.hpp"
#include "SceneManager.hpp"

class InterfaceContent;
class Client;

typedef std::function<bool(Client&)> ResolveableResponder;

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

	void addInputController(bool cascade, bool drawDuringCascade, InputController);

protected:
	std::shared_ptr<Ship> ship;
	std::shared_ptr<Grid> currentGrid;

	std::weak_ptr<Ship> selectedShip;

	Vec2_i raw_mouse_screen;

	void OnMessage(net::message<NetMsgType> msg) override;

private:
	bool isWindowSelected = false;
	bool showNewWindowModal = false;
	float avgPackets = 0;
	
	//to keep client listeners alive since observer dosent handle it
	std::vector<std::shared_ptr<Events::ListenerBase>> listeners;
	
	//all open interfaces, seems like a bette rway to do things than
	//	dynamic casting
	std::vector<std::weak_ptr<InterfaceContent>> interfaceWindows;

	struct InConOptions {
		InputController ic;
		/** should this cascade to its predecessor */
		bool cascade;
		/** should this be drawn if its cascaded too */
		bool drawDuringCascade;
	};

	//stack of controllers, last thing is the active one
	std::vector<InConOptions> InputControllers;

	Component mainContainer;
	Component windowContainer;
	Component clientStats;

	/*
	* functions here are called every update, return true if it can be removed.
	* doubles as a nonblocking way to handle network requests, think ajax but 
	*	worse.
	*/
	std::vector<ResolveableResponder> unresolvedResponders;

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
	std::vector<std::weak_ptr<GameObject>> selectedObjects;

	/****************************************************************
	* game controlls
	*  - also see client events for closely related things
	****************************************************************/

	std::shared_ptr<Ship> GetSelectedShip() const;
	std::vector<std::shared_ptr<Ship>> GetSelectedShips() const;
};
