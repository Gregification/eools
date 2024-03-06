#include <stdio.h>
#include <string.h>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include "Server.hpp"
#include "Client.hpp"

using namespace ftxui;

void promptForConnectionMethod();

static Scene *app;
static auto screen = ScreenInteractive::Fullscreen();

int main(int argc, char *argv[]){
    promptForConnectionMethod();

    if(app){
        auto screenProcess = [](){screen.Loop(app->renderer());};
        std::thread screenRender(screenProcess);
        screenRender.join();
    }
    else
        std::cout << "\a"; 

    return EXIT_SUCCESS;
}

void promptForConnectionMethod() {
    std::vector<std::string> options{
        "join",
        "host",
        "exit"
    };
    int selected = 0;
    MenuOption option;
    option.on_enter = [&]{
        switch (selected){
            case 0:
                app = new Client();
                break;
            case 1: 
                app = new Server();
                break;
            default: break;
        }

        screen.Exit();
    };

    auto connect_menu = Menu(&options, &selected, option);
    
    auto renderer = Renderer(connect_menu, [&]{
        return vbox({
            text("E.O.O.L.S") | color(Color::Red3Bis) | bold,
            text(""),text(""),
            connect_menu->Render() | center
        }) | center | borderLight;
    });

    screen.Loop(renderer);
}