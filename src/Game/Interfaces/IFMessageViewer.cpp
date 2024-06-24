#include "IFMessageViewer.hpp"

IFMessageViewer::IFMessageViewer() {
	content = ftxui::Container::Vertical({});

	PostMessage("brought to you by Interfaces/IFMessageViewer");

	Add(content);
}

void IFMessageViewer::PostMessage(std::string txt) {
	static bool toggle;

	int rows = ftxui::Terminal::Size().dimy;
	if (rows > 0 && rows <= content->ChildCount())
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
