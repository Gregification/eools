#pragma once

#include "InterfaceContent.hpp"

class IFHelm;

#include "../../GameObjects/Ship.hpp"

/*
	ui for controlling & readout of a ship
*/
class IFHelm : public InterfaceContent {
public:
	IFHelm();

	void setShip(std::shared_ptr<Ship>);

	std::weak_ptr<Ship> getShip() const;

protected:
	std::weak_ptr<Ship> _ship;
	
	ftxui::Component controls;

	ftxui::Component getShipView() const;

private:
	Component GenControls(Ship& s);
};