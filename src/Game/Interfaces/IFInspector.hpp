#pragma once

#include "InterfaceContent.hpp"
#include "../../GameObject.hpp"

class IFInspector : public InterfaceContent {
public:
	IFInspector() {
		Add(Renderer([&] {
			std::string name, id;
			int targType = -1;

			if (auto go = selectedObject.lock()) {
				id		= go->id();
				name	= go->getDisplayName();
				targType = static_cast<int>(go->GetClassId());
			} else {
				id		= "N/A";
				name	= "no selection";
			}

			return flexbox(
				{ 
					text(name),
					text(id),
					text("targType:" + std::to_string(targType))
				});
		}));
	}

	void inspectObject(std::weak_ptr<GameObject>);
	

private:
	std::weak_ptr<GameObject> selectedObject;
};