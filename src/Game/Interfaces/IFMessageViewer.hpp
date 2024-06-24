#pragma once

#include "InterfaceContent.hpp"

class IFMessageViewer : public InterfaceContent {
	public:
		IFMessageViewer();
				
		void PostMessage(std::string);
			
		void Refresh() override;
		void OnDelete() override;
		void OnHide() override;

	protected:
		ftxui::Component content;
};
