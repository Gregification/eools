#pragma once

#include <ftxui/component/component.hpp>
#include "../../better-enums/enum.h"

//wrapper class for windowed content thats shown on the play screen
class InterfaceContent : public ftxui::ComponentBase {
public:

	//refresh content
	virtual void Refresh() = 0;

	//called once when selected
	virtual void OnSelect() { Refresh(); }

	//callled once when removed
	virtual void OnDelete() = 0;

	//called once if it was just formerly the focoused tab
	virtual void OnHide() = 0;

	static const int NUM_INTERFACES = 1;
	typedef std::array<std::pair<std::string,std::string>, NUM_INTERFACES > PublicInterfacesType;
	static const PublicInterfacesType publicInterfaces;
};
