#pragma once

#include "App.hpp"

#include "NetMessageType.h"
#include <eol_net.hpp>

class Server : public App, public net::server_interface<NetMsgType> {
public:
	Server(uint16_t listeningPort) : net::server_interface<NetMsgType>(listeningPort) {

	}

    void run(ScreenInteractive& screen);
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
	std::thread screenThread;

private:
	std::recursive_mutex renderMutex;
	ftxui::Component renderer;
	ftxui::Elements messages;
};