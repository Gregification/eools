#include "IFOptions.hpp"

#include "../../ftxui extras/scroller.hpp"
#include "../Input Controllers/general/ICSelectGridRectangle.hpp"
#include "../Input Controllers/nav/ICNavAlign.hpp"

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
        std::vector<Component> comps;

        for (int i = 1; i < Navigation::TRAVEL_STATE::_size_constant; i++) {
            typedef Navigation::TRAVEL_STATE TS;
            TS ts = TS::_from_index(i);

            comps.push_back(
                Button(
                    ts._to_string(),
                    [mousePos, ts] {
                        using namespace Events;
                        ClientEvent::observer.invokeEvent<ResolveableResponder>(
                            ClientEvent::CLIENT_EVENT::ADD_RESOLVEABLE_RESPONDER,
                            [mousePos, ts](Client& postClient) -> bool{

                                //holie molie this was a mess to set up the imports for.
                                //plz confirm functionality after any change

                                //add to client
                                postClient.addInputController(
                                    true,
                                    true,
                                    Navigation::stateToNavigatorIC[ts]()
                                );

                                return true;
                            });
                    }
                    , ButtonOption::Ascii())
            );

        }

        movementInner = Inner(comps);
    }


	auto comp =  //Scroller( //disables the collapsibles for some reason
        Container::Vertical({
        Collapsible("Displays", Inner({windowsInner})),
		Collapsible("Navigation", movementInner),
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
