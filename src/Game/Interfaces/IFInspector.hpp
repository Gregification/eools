#pragma once

#include "InterfaceContent.hpp"
#include "../../GameObject.hpp"

class IFInspector : public InterfaceContent {
public:
	IFInspector();

	void setInspectedObject(std::weak_ptr<GameObject>);
	
private:
	std::weak_ptr<GameObject> selectedObject;
};