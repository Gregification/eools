#pragma once

#include "InterfaceContent.hpp"
#include "../../GameObject.hpp"

class IFInspector : public InterfaceContent {
public:
	IFInspector();

	void setInspectedObject(std::weak_ptr<GameObject>);
	std::weak_ptr<GameObject> getInspectedObject() const;
	
	int currentSelectionIdx = 0;
private:
	std::weak_ptr<GameObject> selectedObject;
};