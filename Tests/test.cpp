#include <catch2/catch_all.hpp>

#include <iostream>

#include "../src/GameObjectFactory.hpp"
#include <cassert>

int main() {
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "tests done using the Catch2 framework." << std::endl;
	std::cout << "	> Catch2 : https://github.com/catchorg/Catch2" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;

	return Catch::Session().run();
}

//int main() {
//	assert(GameObjectFactory::nextIdx == 3);
//
//	// should pass because the vector should contain 3 elements
//	assert(GameObjectFactory::getClassListSize() == 3);
//
//	return 0;
//}