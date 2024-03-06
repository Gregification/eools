#pragma once

#include "Scene.hpp"

using namespace ftxui;

class Client : public Scene {
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
