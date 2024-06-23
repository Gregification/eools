#pragma once

#include "InterfaceContent.hpp"

namespace IFMessageViewer {
	using namespace ftxui;

	BETTER_ENUM(MESSAGE_TAG, int,
		ALL,
		DEBUG,
		SHIP,
		ENVIROMENT
	)

	class MessageViewer : InterfaceContent {
		public:
			IFMessageViewer() {
				auto tab_toggle = Toggle(MESSAGE_TAG::_values, &tab_selected);

				for (auto v : MESSAGE_TAG::_values) {
					tag_to_tab[v] = Container::Vertical({"test" + std::to_string(v)});

				}
				
				Add(Container::Vertical({
					tab_toggle,
					tab_container
				}));
			}

			void postMessage(MESSAGE_TAG::_enumerated, std::string);
			
			void setTabSelected(MESSAGE_TAG);
			MESSAGE_TAG getTabSelected();

			void refresh() override;
			void onDelete() override;
			void onHide() override;

		private:
			auto tab_container = Container::Tab({}, &tab_selected);
			int tab_selected = MESSAGE_TAG::ALL;

			std::unordered_map <MESSAGE_TAG::_enumerated, Component> tag_to_tab;
	};
}