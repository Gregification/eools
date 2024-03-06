#include "Server.hpp"

using namespace ftxui;

Server::Server(){
    int i = 0;
    container = Renderer([&] {
        return vbox({
                text("server rendere " + std::to_string(i++)) |
                    color(Color::LightGreenBis),
                gauge(0),
            }) |
            center;
    });
}

Component Server::renderer() { 
    return container;
};

void Server::shutdown() {

}
