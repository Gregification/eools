#pragma once

#include "App.hpp"

class Server : public App {
    public:
        Server() = default;
        Component renderer() override {
            return Renderer([&] {
                return vbox({
                        text("server rendere") |
                            color(Color::LightGreenBis),
                        gauge(0),
                    }) |
                    center;
            });
        };
        void shutdown() override{

        };
};