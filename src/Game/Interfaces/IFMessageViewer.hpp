#pragma once

#include <iostream>

#include "InterfaceContent.hpp"
#include "../Events/Events.hpp"

//https://stackoverflow.com/questions/43207703/visual-studio-adding-capital-a-to-function-name-in-error-message
#ifdef RegisterClass
#undef RegisterClass
#endif

class IFMessageViewer : public InterfaceContent {
	public:
		IFMessageViewer();
		
		//see ifdef statement at top of this class deceleration
		void Post_Message(std::string, ftxui::ElementDecorator = ftxui::nothing);
			
		void Refresh() override;
		void OnDelete() override;
		void OnHide() override;

		bool ToggleColor = true;

	protected:
		ftxui::Component content;

	private:
		bool toggle;
		std::shared_ptr<Events::Listener<std::string>> listener;
};
