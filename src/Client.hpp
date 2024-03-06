#pragma once

#include "App.hpp"

using namespace ftxui;

class Client : public App {
    public:
        Client() = default;
        Component renderer() override {
            return Renderer([&] {
                return vbox({
                        text("client rendere") |
                            color(Color::LightGreenBis),
                        gauge(0),
                    }) |
                    center;
            });
        };
        void shutdown() override {

        };
};
