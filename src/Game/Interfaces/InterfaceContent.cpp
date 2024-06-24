#include "InterfaceContent.hpp"

#include "InterfaceInclude.hpp"

const InterfaceContent::PublicInterfacesType InterfaceContent::publicInterfaces = { {
	{"Message Viewer",[]() {
		return ftxui::Make<IFMessageViewer>();
	}},
}};
