#pragma once

#include "Scene.hpp"

class Server : public Scene {
    public:
        Server();
        Component renderer() override;
        void shutdown() override;
    protected:
        Component container = Renderer([&] {
            static int i = 0;
            return vbox({
                    text("server rendere " + std::to_string(i++)) |
                        color(Color::LightGreenBis),
                    gauge(0),
                }) |
                center;
        });
};