#include "IFMessageViewer.hpp"

IFMessageViewer::IFMessageViewer() {
	content = ftxui::Container::Vertical({});

	PostMessage(ftxui::text("messages appear here"));

	Add(content);
}

void IFMessageViewer::PostMessage(ftxui::Element && line) {
	static bool toggle;

	if (messageBufferSize >= content->ChildCount())
		content->ChildAt(0)->Detach();

	auto r = ftxui::Renderer([element = move(line)] { return element; });
	if(toggle = !toggle) r |= ftxui::bgcolor(ftxui::Color::GrayLight);

	content->Add(move(r));
}

void IFMessageViewer::Refresh() {
	
}

void IFMessageViewer::OnDelete() {
	
}

void IFMessageViewer::OnHide() {
	

}
