#include <iostream>

#include "Server.hpp"
#include "QueueClient.hpp"
#include "GameStructs.hpp"

using namespace ftxui;

void prepScene();

App *app;
ScreenInteractive screen = ScreenInteractive::Fullscreen();
uint16_t serverPort = SERVER_PORT;

int main(int argc, char *argv[]) {

    if (argc > 1)
        serverPort = std::stoi(argv[1]);

    prepScene();

    if (app) app->run(screen);
    else std::cout << "\a";

    delete app;
    screen.Exit();
    
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
                case 0: app = new QueueClient();
                    break;
                case 1: app = new Server(serverPort);
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