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

	template<typename T>
	std::shared_ptr<Events::Listener<T>> addListener(std::shared_ptr<Events::Listener<T>>);

	//refresh content
	virtual void Refresh() {}
	/*called once when selected*/
	virtual void OnFocus() { Refresh(); }
	/*called once if it was just formerly the focoused tab*/
	virtual void OnUnfocus() {}

	static const int _pitsize = 4;
	typedef std::array<std::pair<
			std::string,
			std::function<std::shared_ptr<InterfaceContent>(Client&)>
	>, _pitsize>
		PublicInterfacesType;

	static const PublicInterfacesType publicInterfaces;

	std::vector<std::shared_ptr<Events::ListenerBase>> listeners;

};
