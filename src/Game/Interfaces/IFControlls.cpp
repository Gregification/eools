#include <ftxui/dom/table.hpp>

#include "IFControlls.hpp"
#include "../Events/Events.hpp"

#include "../../ftxui extras/scroller.hpp"

std::vector<std::vector<std::string>>
	IFControlls::options{};

IFControlls::IFControlls() {
	Refresh();

	Add(ftxui::Scroller(ftxui::Renderer([&] {
		auto table = ftxui::Table(options);
		
		table.SelectAll().Border(ftxui::BorderStyle::EMPTY);
		table.SelectAll().Decorate(ftxui::xflex);

		return table.Render();
	})));
}

void IFControlls::Refresh() {
	options.clear();

	for(int i = 0; i < Events::KeyBinds::BE_CONTROL_EVENT::_size(); i++) {
		auto event = Events::KeyBinds::BE_CONTROL_EVENT::_from_index(i);
		
		std::string ctrl = "unbound";

		//linear search for the keybind
		for (auto [k, v] : Events::KeyBinds::KeyMap) {
			for(auto e : v)
				if (e == (int)event) {
					ctrl = k.character();
					goto cont;
				}
		}
	cont:

		options.push_back({ event._to_string(), "\""+ctrl+ "\""});
	}
}
