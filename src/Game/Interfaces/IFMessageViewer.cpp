#include "IFMessageViewer.hpp"
#include <iostream>

IFMessageViewer::IFMessageViewer() 
	: listener([&](std::string msg) { PostMessage(msg); }){

	content = ftxui::Container::Vertical({}) | ftxui::focusPositionRelative(0, 1) | ftxui::yframe;

	PostMessage("brought to you by Interfaces/IFMessageViewer");


	Add(content);

	Events::ClientEvent::observer.AddListenerToEvent(Events::ClientEvent::CLIENT_EVENT::EVENT_MESSAGE,
		std::make_shared<Events::Listener<std::string>>(listener)
	);
}

void IFMessageViewer::PostMessage(std::string txt) {
	const int rows = ftxui::Terminal::Size().dimy;
	if(rows > 0)
		while (rows <= content->ChildCount())
			content->ChildAt(0)->Detach();

	ftxui::Element ele = ftxui::paragraphAlignLeft(move(txt));

	auto r = ftxui::Renderer([ele] { return ele; });
	if(toggle = !toggle) r |= ftxui::bgcolor(ftxui::Color::GrayDark);

	content->Add(move(r));
}

void IFMessageViewer::Refresh() {
	
}

void IFMessageViewer::OnDelete() {
	this->Detach();
}

void IFMessageViewer::OnHide() {
	

}
