#include "InterfaceContent.hpp"
#include "../Events/Events.hpp"
#include "InterfaceInclude.hpp"

using namespace Events;

const InterfaceContent::PublicInterfacesType InterfaceContent::publicInterfaces = { {
	{"Message Viewer",[](Client&) {
		auto mv = ftxui::Make<IFMessageViewer>();

		if(rand() < RAND_MAX / 50)
			mv->Post_Message("We've been trying to reach you about your ship's extended warranty...");

		Events::ClientEvent::observer.AddListenerToEvent(
			Events::ClientEvent::CLIENT_EVENT::EVENT_MESSAGE,
			mv->addListener(Events::MakeListener<std::string>(
				[p = mv](std::string msg) {
					p->Post_Message(msg);
				}
			))
		);

		return mv;
	}},
	{"Helm",[](Client& c) {
		auto mv = ftxui::Make<IFHelm>();

		ClientEvent::observer.AddListenerToEvent(
			ClientEvent::CLIENT_EVENT::ON_SHIP_OPERABLE_SHIP_FOCOUS,
			mv->addListener(Events::MakeListener< std::shared_ptr<Ship>>(
				[p = mv](std::shared_ptr<Ship> s) {
					p->setShip(s);
				}
			)
		));

		mv->setShip(c.getShip());

		return mv;
	}},
	{"Controlls",[](Client& c) {
		return ftxui::Make<IFControlls>();
	}},
	{"Inspector",[](Client& c) {
		auto fi = ftxui::Make<IFInspector>();

		ClientEvent::observer.AddListenerToEvent(
			ClientEvent::CLIENT_EVENT::ON_GAMEOBJECT_SELECT,
			fi->addListener(Events::MakeListener<GameObjPtr>(
				[p = fi] (auto g){
					p->setInspectedObject(g);
				}))
		);

		return fi;
	}},
} };

template<typename ARG>
std::shared_ptr<Events::Listener<ARG>> InterfaceContent::addListener(std::shared_ptr<Events::Listener<ARG>> ll) {
	listeners.push_back(ll);
	return ll;
}