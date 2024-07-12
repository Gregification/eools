#pragma once

#include <typeindex>
#include <functional>

#include <ftxui/component/component.hpp>
#include "../../better-enums/enum.h"
#include "../../Client.hpp"

//wrapper class for windowed content thats shown on the play screen
class InterfaceContent : public ftxui::ComponentBase {
public:
	virtual ~InterfaceContent() = default;

	//refresh content
	virtual void Refresh() {};

	/*called once when selected*/
	virtual void OnSelect() { Refresh(); }
	/*callled once when removed*/
	virtual void OnDelete() {};
	/*called once if it was just formerly the focoused tab*/
	virtual void OnHide() {};

	static const int NUM_INTERFACES = 2;//yeah
	typedef std::array<
		std::pair<
			std::string,
			std::function<std::shared_ptr<InterfaceContent>(Client&)>
		>, NUM_INTERFACES 
	> PublicInterfacesType;

	static const PublicInterfacesType publicInterfaces;
};
