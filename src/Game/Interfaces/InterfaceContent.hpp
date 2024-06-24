#pragma once

#include <typeindex>
#include <functional>

#include <ftxui/component/component.hpp>
#include "../../better-enums/enum.h"

//wrapper class for windowed content thats shown on the play screen
class InterfaceContent : public ftxui::ComponentBase {
public:
	virtual ~InterfaceContent() = default;

	//refresh content
	virtual void Refresh() = 0;

	//called once when selected
	virtual void OnSelect() { Refresh(); }

	//callled once when removed
	virtual void OnDelete() = 0;

	//called once if it was just formerly the focoused tab
	virtual void OnHide() = 0;

	static const int NUM_INTERFACES = 1;//yeah
	typedef std::array<std::pair<std::string, std::function<std::shared_ptr<InterfaceContent>()>>, NUM_INTERFACES > PublicInterfacesType;
	static const PublicInterfacesType publicInterfaces;
};
