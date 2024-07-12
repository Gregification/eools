#include "InterfaceContent.hpp"
#include "../Events/Events.hpp"

#include "InterfaceInclude.hpp"


const InterfaceContent::PublicInterfacesType InterfaceContent::publicInterfaces = { {
	{"Message Viewer",[](Client&) {
		auto mv = ftxui::Make<IFMessageViewer>();
		mv->Post_Message("non scrollable text");

		mv->listener = Events::MakeListener<std::string>([&](std::string msg) {
				mv->Post_Message(msg);
		});
		Events::ClientEvent::observer.AddListenerToEvent(
			Events::ClientEvent::CLIENT_EVENT::EVENT_MESSAGE,
			mv->listener
		);

		return mv;
	}},
	{"Helm",[](Client&) {
		auto mv = ftxui::Make<IFHelm>();

		return mv;
	}},
} };
