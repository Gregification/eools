#include "IFOptions.hpp"
#include "../../ftxui extras/scroller.hpp"


IFOptions::IFOptions(Vec2_i mousePos, GameObjPtr go, Client& client) {
    Component windowsInner;
    {
        std::vector<Component> comps;
        for (auto& v : InterfaceContent::publicInterfaces) {
            comps.push_back(
                Button( v.first,
                    [mousePos, v] {
                        using namespace Events;
                        ClientEvent::observer.invokeEvent<std::function<Component(Client&)>>(
                            ClientEvent::CLIENT_EVENT::ADD_TO_WINDOW_CONTAINER,
                            [mousePos, v](auto& c) {
                                return Window({
                                    .inner = v.second(c),
                                    .title = v.first,
                                    .left = mousePos.x,
                                    .top = mousePos.y,
                                });
                            });
                    }
                , ButtonOption::Ascii())
            );
        }

        windowsInner = Inner(comps);
    }

    Component movementInner;
    {
        movementInner = Inner({
                        Collapsible("Collapsible 1.1.1", Button("meow", [] {})),
                        Collapsible("Collapsible 1.1.2", Empty()),
                        Collapsible("Collapsible 1.1.3", Empty()),
            });
    }


	auto comp =  //Scroller( //disables the collapsibles for some reason
        Container::Vertical({
        Collapsible("Windows", Inner({windowsInner})),
		Collapsible("Movement", movementInner),
		Collapsible("C", 
            Inner({
                Collapsible("Collapsible 1.1",
                    Inner({
                        Collapsible("Collapsible 1.1.1", Button("meow", [] {})),
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
