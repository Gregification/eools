#include <catch2/catch_all.hpp>

#include <iostream>

int main() {
	std::cout << "testing EOOLS using the Catch2 framework." << std::endl << "\t - Catch2 : https://github.com/catchorg/Catch2" << std::endl;
	return Catch::Session().run();
}