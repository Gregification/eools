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
			MessageViewer() {
				auto tag_names = std::vector<std::string>(MESSAGE_TAG::_size());
				for (auto v : MESSAGE_TAG::_values())
					tag_names.push_back(v._to_string());

				auto tab_toggle = Toggle(&tag_names, &tab_selected);

				for (auto v : MESSAGE_TAG::_values()) {
					tag_to_tab[v] = Container::Tab({
							Renderer([&] {
								return vbox({
										text(v._to_string())
									});
								})
						}, &tab_selected);
				}
				
				Add(Container::Vertical({
					tab_toggle,
					tab_container
				}));
			}

			void PostMessage(MESSAGE_TAG::_enumerated, std::string);
			
			void SetTabSelected(MESSAGE_TAG);
			MESSAGE_TAG GetTabSelected() const;

			void Refresh() override;
			void OnDelete() override;
			void OnHide() override;

		private:
			Component tab_container = Container::Tab({}, &tab_selected);
			int tab_selected = MESSAGE_TAG::ALL;

			std::unordered_map <MESSAGE_TAG::_enumerated, Component> tag_to_tab;
	};
}