#include "IFMessageViewer.hpp"
#include <iostream>

IFMessageViewer::IFMessageViewer() {

	content = ftxui::Container::Vertical({});

	listener = Events::MakeListener<std::string>([&](std::string msg) { Post_Message(msg); });
	
	//Post_Message("brought to you by Interfaces/IFMessageViewer");

	Add(content | ftxui::focusPositionRelative(0, 1) | ftxui::yframe);
}

void IFMessageViewer::Post_Message(std::string txt, ftxui::ElementDecorator decor) {
	const int rows = ftxui::Terminal::Size().dimy - 2;
	if(rows > 0)
		while (rows <= content->ChildCount())
			content->ChildAt(0)->Detach();

	//appends the last 3 second time stamp 
	ftxui::Element ele = ftxui::paragraphAlignLeft(
		std::to_string(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 10000 ) 
		+ ": " 
		+ txt);

	ele |= decor;

	auto r = ftxui::Renderer([ele] { return ele; });
	if(ToggleColor && (toggle = !toggle)) r |= ftxui::bgcolor(ftxui::Color::GrayDark);
	content->Add(r);
}

void IFMessageViewer::Refresh() {
	
}

void IFMessageViewer::OnDelete() {
	this->Detach();
}

void IFMessageViewer::OnHide() {
	

}
