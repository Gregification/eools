#pragma once

#include "App.hpp"

#include "NetMessageType.h"
#include <eol_net.hpp>

class Server : public App, public net::server_interface<NetMessageType> {
public:
    Server(uint16_t listeningPort) : net::server_interface<NetMessageType>(listeningPort) {
	
	}

    void run(ScreenInteractive& screen);
protected:
	//returns true/accept or false/decline regarding the connection. think Swing predicate filters
	virtual bool onClientConnect(std::shared_ptr<net::connection<NetMessageType>> client) override;

	virtual void onClientDisconnect(std::shared_ptr<net::connection<NetMessageType>> client) override;

	//on message from specific given client
	virtual void OnMessage(std::shared_ptr<net::connection<NetMessageType>> client, net::message<NetMessageType>& msg) override;

	//for user purposes, good luck tring to use this for anyhting otherwise. rip
	virtual void onError(std::string message) override;

	//on notable evets such as connection being denied, user joining and so on
	virtual void onEvent(std::string message) override;

protected:
	std::thread screenThread;
	std::vector<std::string> messages;
};