#pragma once

#include "Server.hpp"
#include "ftxui/dom/canvas.hpp"                    // for Canvas

Server::Server() {
	Canvas c = Canvas(500, 500);
}

void Server::run(Screen screen) {
	//screenRenderer = std::make_unique(new std::thread());
}
