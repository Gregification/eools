#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

using namespace ftxui;

class App {
    public:
        App() = default;
        virtual void run(ScreenInteractive& screen) {};
    protected:
        std::unique_ptr<std::thread> screenRenderer;
};