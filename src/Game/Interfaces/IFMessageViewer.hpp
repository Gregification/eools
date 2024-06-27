#pragma once

#include <iostream>

#include "InterfaceContent.hpp"
#include "../Events/Events.hpp"

class IFMessageViewer : public InterfaceContent {
	public:
		IFMessageViewer();
		~IFMessageViewer() {
			std::cout << "\a" << std::endl;
			
		};
				
		void PostMessage(std::string);
			
		void Refresh() override;
		void OnDelete() override;
		void OnHide() override;

	protected:
		ftxui::Component content;

	private:
		bool toggle;
		Events::Listener<std::string> listener;
};
