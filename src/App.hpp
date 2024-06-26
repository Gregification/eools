#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

using namespace ftxui;

class App {
    public:
        App() = default;
        virtual void run(ScreenInteractive&) = 0;
    protected:
        std::thread screenThread;
        std::recursive_mutex renderMutex;
};