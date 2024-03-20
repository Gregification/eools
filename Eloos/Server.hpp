#pragma once

#include <eol_net.hpp>
#include <set>

#include "App.hpp"
#include "NetMessageType.h"
#include "GameMap.hpp"

//dollar store singleton pattern supported with certified trust-me-bro design
class Server : public App, public net::server_interface<NetMsgType> {
public:
	Server(uint16_t listeningPort)
			: net::server_interface<NetMsgType>(listeningPort),
			partitionCounter(nIDCounter)
		{
		messages.push_back(text(std::format("see NetMessageType.h for message types.")) | color(Color::DarkSeaGreen3) | underlined);
		
		LOCAL_PARITION.nxt = LOCAL_PARITION.min = 0;
		LOCAL_PARITION.max = STD_PARTITION_SIZE;
	}

    void run(ScreenInteractive& screen) override;
protected:
	//returns true/accept or false/decline regarding the connection. think Swing predicate filters
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

	ftxui::Elements messages;
	std::set<std::string> blacklist_ip;

private:
	GameMap gmap;
	uint32_t partitionCounter;
};