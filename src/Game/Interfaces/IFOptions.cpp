#include "IFOptions.hpp"

IFOptions::IFOptions(Vec2_i mousePos, Client& client) {
	auto comp = ftxui::Container::Vertical({
		Collapsible("A", Empty()),
		Collapsible("B", Empty()),
		Collapsible("C", 
            Inner({
                Collapsible("Collapsible 1.1",
                    Inner({
                        Collapsible("Collapsible 1.1.1", Empty()),
                        Collapsible("Collapsible 1.1.2", Empty()),
                        Collapsible("Collapsible 1.1.3", Empty()),
                })),
                Collapsible("Collapsible 1.2",
                    Inner({
                        Collapsible("Collapsible 1.2.1", Empty()),
                        Collapsible("Collapsible 1.2.2", Empty()),
                        Collapsible("Collapsible 1.2.3", Empty()),
                })),
                Collapsible("Collapsible 1.3",
                    Inner({
                        Collapsible("Collapsible 1.3.1", Empty()),
                        Collapsible("Collapsible 1.3.2", Empty()),
                        Collapsible("Collapsible 1.3.3", Empty()),
                    })),
        })),
	});

	Add(comp);
}

void IFOptions::OnUnfocus() {
    Detach();
}
