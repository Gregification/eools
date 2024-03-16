#pragma once

#include <eol_net.hpp>

#include "App.hpp"
#include "NetMessageType.h"

class Client : public App, public net::client_interface<NetMsgType> {
public:
	Client() {
		/*auto canvas_element = canvas([](Canvas& c) {
			c.DrawPointLine(1, 1, c.width() - 1, c.height() - 1, Color::Red);
			c.DrawBlock(0, 0, true, Color::Green);

			});
		auto flexible_canvas_element = canvas_element | flex;

		screen.Loop(Renderer([&] {
			return vbox({
				flexible_canvas_element,
				});
			}));*/
	}

	void run(ScreenInteractive& screen) override;
};