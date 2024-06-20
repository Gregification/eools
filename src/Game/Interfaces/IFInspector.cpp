#include "IFInspector.hpp"

void IFInspector::inspectObject(std::weak_ptr<GameObject> go) {
	this->selectedObject = go;
	
}