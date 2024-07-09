//#include <catch2/catch_all.hpp>

//#include <iostream>

#include "../src/GameObjectFactory.hpp"
#include <cassert>

int main() {
	assert(GameObjectFactory::nextIdx == 3);

	//passes
	// should fail because the vector should contain 3 elements
	assert(GameObjectFactory::getClassListSize() == 0);

	return 0;
}

//int main() {
//	std::cout << "---------------------------------------------------------------" << std::endl;
//	std::cout << "testing EOOLS using the Catch2 framework." << std::endl;
//	std::cout << "	> Catch2 : https://github.com/catchorg/Catch2" << std::endl;
//	std::cout << "---------------------------------------------------------------" << std::endl;
//
//	return Catch::Session().run();
//}