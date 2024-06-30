#pragma once

#include <set>

#include "NetCommon/eol_net.hpp"
#include "App.hpp"
#include "NetMessageType.hpp"
#include "GameObjects/Ship.hpp"
#include "Game/Interfaces/IFMessageViewer.hpp"

//off brand dollar store singleton pattern supported with certified trust-me-bro design
class Server : public App, public net::server_interface<NetMsgType> {
public:
	Server(uint16_t listeningPort)
		: net::server_interface<NetMsgType>(listeningPort),
		messageViewer(ftxui::Make<IFMessageViewer>())
	{
		messageViewer->ToggleColor = false;

		LOCAL_PARITION.nxt = LOCAL_PARITION.min = 0;
		LOCAL_PARITION.max = STD_PARTITION_SIZE;
	}

    void run(ScreenInteractive&) override;
protected:
	//returns true/accept or false/decline regarding the connection. Swing predicate filters
	virtual bool onClientConnect(std::shared_ptr<net::connection<NetMsgType>> client) override;

	virtual void onClientDisconnect(std::shared_ptr<net::connection<NetMsgType>> client) override;

	//on message from specific given client
	virtual void OnMessage(std::shared_ptr<net::connection<NetMsgType>> client, net::message<NetMsgType>& msg) override;

	//for user purposes, good luck tring to use this for anyhting otherwise. rip
	virtual void onError(std::string message) override;

	//on notable evets such as connection being denied, user joining and so on
	virtual void onEvent(std::string message) override;	

protected:
	ftxui::Component renderer;

	std::shared_ptr<IFMessageViewer> messageViewer;
	std::set<std::string> blacklist_ip;

	void primeGameMap();

private:
	std::unordered_map<Class_Id, Instance_Id> clientId_to_gridId_map;
	std::unordered_map<Class_Id, ConnectionStatus> connectionStatus;
};