#include "GOInit.hpp"

#include "../GameObjects/Ship.hpp"
#include "../GameObjects/Grid.hpp"

void GOInit::init() {
	IdGen<Ship>::init();
	IdGen<Grid>::init();
	IdGen<GameObject>::init();
}
