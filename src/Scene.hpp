#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

using namespace ftxui;

class Scene {
    public:
        virtual Component renderer() { return NULL;}
        virtual void shutdown() {}
        // virtual void run() {}
};