#include "IFOptions.hpp"
#include "../../ftxui extras/scroller.hpp"


IFOptions::IFOptions(Vec2_i mousePos, Client& client) {
    Component windowsInner;
    {
        std::vector<Component> comps;
        for (auto& v : InterfaceContent::publicInterfaces) {
            comps.push_back(
                Button(
                    v.first,
                    [&] {
                        client.windowContainer->Add(Window({
                            .inner = v.second(client),
                            .title = v.first,
                            .left = mousePos.x,
                            .top = mousePos.y,
                        }));
                    }
                )
            );
        }

        windowsInner = Inner(comps);
    }


	auto comp =  Scroller(Container::Vertical({
        //Collapsible("Windows", Inner({windowsInner})),
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
	}));

	Add(comp);
}

void IFOptions::OnUnfocus() {
    Detach();
}
