#pragma once

#include <iostream>

#include "Server.hpp"
#include "Client.hpp"

void prepScene();

using namespace ftxui;

static App *app;
static ScreenInteractive screen = ScreenInteractive::Fullscreen();

int main() {
    prepScene();

    if (app) app->run(screen);
    else std::cout << "\a";

    delete app;
	return EXIT_SUCCESS;
}

void prepScene() {
    std::vector<std::string> options{
        "join",
        "host",
        "exit"
    };
    int selected = 0;
    MenuOption option;
    option.on_enter = [&] {
            switch (selected) {
                case 0: app = new Client();
                    break;
                case 1: app = new Server();
                    break;
                default: break;
            }

            screen.Exit();
        };

    auto ui_menu = Menu(&options, &selected, option);

    auto renderer = Renderer(ui_menu, [&] {
        return vbox({
            text("E.O.O.L.S") | color(Color::Green3Bis) | bold,
            text(""),text(""),
            ui_menu->Render() | center
            }) | center | borderLight;
        });

    screen.Loop(renderer);
}