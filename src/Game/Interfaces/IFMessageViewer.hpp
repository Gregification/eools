#pragma once

#include "InterfaceContent.hpp"

class IFMessageViewer : public InterfaceContent {
	public:
		IFMessageViewer();

		int messageBufferSize = 20;
		
		void PostMessage(ftxui::Element&&);
			
		void Refresh() override;
		void OnDelete() override;
		void OnHide() override;

	protected:
		ftxui::Component content;
};
