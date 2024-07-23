#pragma once 

#include "InterfaceContent.hpp"

class IFOptions : public InterfaceContent {
public:
	IFOptions(Vec2_i mousePos, GameObjPtr, Client&);

	void OnUnfocus() override;

	//funcitons bellow were cpoied form ftxui/examples/component/collapsible.cpp
private:
	static Component Inner(std::vector<Component> children) {
		Component vlist = Container::Vertical(std::move(children));
		return Renderer(vlist, [vlist] {
			return hbox({
				text(" "),
				vlist->Render(),
				});
			});
	}

	static Component Empty() {
		return std::make_shared<ComponentBase>();
	}
};