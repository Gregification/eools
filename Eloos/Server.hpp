#pragma once

#include "App.hpp"

class Server : public App {
public:
    Server();
    void run(Screen screen) override;
protected:
};