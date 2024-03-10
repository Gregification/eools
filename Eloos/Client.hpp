#pragma once

#include "App.hpp"

class Client : public App {
public:
	Client();
	void run(Screen screen) override;
};