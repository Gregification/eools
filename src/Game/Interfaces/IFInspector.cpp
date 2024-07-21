#include "IFInspector.hpp"

void IFInspector::setInspectedObject(std::weak_ptr<GameObject> go) {
	this->selectedObject = go;
}