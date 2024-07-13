#include "InterfaceContent.hpp"
#include "../Events/Events.hpp"

#include "InterfaceInclude.hpp"


const InterfaceContent::PublicInterfacesType InterfaceContent::publicInterfaces = { {
	{"Message Viewer",[](Client&) {
		auto mv = ftxui::Make<IFMessageViewer>();
		mv->Post_Message("non scrollable text");

		Events::ClientEvent::observer.AddListenerToEvent( //behold, a chunk
			Events::ClientEvent::CLIENT_EVENT::EVENT_MESSAGE,
			mv->addListener(Events::MakeListener<std::string>(
				[&](std::string msg) {
					mv->Post_Message(msg);
				}
			)
		));

		return mv;
	}},
	{"Helm",[](Client& c) {
		auto mv = ftxui::Make<IFHelm>();

		Events::ClientEvent::observer.AddListenerToEvent(
			Events::ClientEvent::CLIENT_EVENT::ON_SHIP_OPERABLE_SHIP_FOCOUS,
			mv->addListener(Events::MakeListener< std::shared_ptr<Ship>>(
				[&](std::shared_ptr<Ship> s) {
					mv->setShip(s);
				}
			)
		));

		mv->setShip(c.getShip());

		return mv;
	}},
} };

template<typename ARG>
std::shared_ptr<Events::Listener<ARG>> InterfaceContent::addListener(std::shared_ptr<Events::Listener<ARG>> ll) {
	listeners.push_back(ll);
	return ll;
}