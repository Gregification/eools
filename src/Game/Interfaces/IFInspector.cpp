#include "IFInspector.hpp"

IFInspector::IFInspector() {
	Add(Renderer([&] {
		std::string name, id;
		int targType = -1;

		if (auto go = selectedObject.lock()) {
			id = std::to_string(go->id());
			name = go->getDisplayName();
			targType = static_cast<int>(go->GetClassId());
		}
		else {
			id = "N/A";
			name = "no selection";
		}

		return vbox(
			{
				text(name),
				text(id),
				text("targType:" + std::to_string(targType))
			});
		}));
}

void IFInspector::setInspectedObject(std::weak_ptr<GameObject> go) {
	this->selectedObject = go;
}