#include "InterfaceContent.hpp"
#include "../../GameObjects/Ship.hpp"

/*
	ui for controlling & readout of a ship
*/
class IFHelm : public InterfaceContent {
public:
	IFHelm();

	void Of(std::shared_ptr<Ship>);

private:
	std::weak_ptr<Ship> of;
};