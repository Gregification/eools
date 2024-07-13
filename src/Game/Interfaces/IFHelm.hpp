#include "InterfaceContent.hpp"
#include "../../GameObjects/Ship.hpp"

/*
	ui for controlling & readout of a ship
*/
class IFHelm : public InterfaceContent {
public:
	IFHelm();

	void Of(std::shared_ptr<Ship>);
	
	std::weak_ptr<Ship> getOf() const;


	std::function<void(std::shared_ptr<Ship>)> listener;

private:
	std::weak_ptr<Ship> _of;
};