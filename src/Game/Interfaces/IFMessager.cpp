#include "IFMessager.hpp"

#include "../../NetMessageType.hpp"
#include "../../Client.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

IFMessager::IFMessager()
{
	using namespace ftxui;

	Component textArea = Input(&chatMessage.str, "");
	
	//absolutely no automatic user id. high trust gameplay(trust that it wont crash)

	textArea |= CatchEvent([&](Event e) -> bool { 
		if (e.is_mouse()) return false;

		if (e == Event::Return) {
			using namespace Events;

			ClientEvent::observer.invokeEvent<ResolveableResponder>(
				ClientEvent::CLIENT_EVENT::ADD_RESOLVEABLE_RESPONDER,
				[=](Client& c) -> bool {					
					Message m;
					m.header.id = NetMsgType::OtherMsg;
					chatMessage.packMessage(m);
					c.Send(std::move(m));

					ClientEvent::observer.invokeEvent(
						ClientEvent::CLIENT_EVENT::EVENT_MESSAGE,
						chatMessage.str
					);

					return true;
				});

			chatMessage.str.clear();	
		}

		return e.is_character() && chatMessage.str.size() > OTHERMSG::ChatMsg::MAX_LEN;
	});

	Add(textArea);
}
