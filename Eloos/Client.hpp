#pragma once

#include "App.hpp"

class Client : public App {
public:
	Client();
	void run(ScreenInteractive& screen);
};