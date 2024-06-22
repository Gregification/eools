#include <catch2/catch_all.hpp>

#include <iostream>

int main() {
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "testing EOOLS using the Catch2 framework." << std::endl;
	std::cout << "	> Catch2 : https://github.com/catchorg/Catch2" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;

	return Catch::Session().run();
}