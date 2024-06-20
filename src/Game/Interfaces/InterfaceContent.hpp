#pragma once

#include <ftxui/component/component.hpp>

//wrapper class for windowed content thats shown on the play screen
class InterfaceContent : public ftxui::ComponentBase {
public:
	virtual void refresh() = 0;
	virtual void onSelect() { refresh(); }
	virtual void onDelete() = 0;
	virtual void onHide() = 0;
};