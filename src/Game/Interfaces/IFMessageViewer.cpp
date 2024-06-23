#include "IFMessageViewer.hpp"

using namespace IFMessageViewer;

void MessageViewer::PostMessage(MESSAGE_TAG::_enumerated, std::string) {

}

void MessageViewer::SetTabSelected(MESSAGE_TAG) {

}

MESSAGE_TAG MessageViewer::GetTabSelected() const {
	return MESSAGE_TAG::_from_index(tab_selected);
}


void MessageViewer::Refresh() {
	
}

void MessageViewer::OnDelete() {
	
}

void MessageViewer::OnHide() {
	

}
