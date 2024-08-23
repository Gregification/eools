#include "IFMessager.hpp"

#include "../../NetMessageType.hpp"
#include "../../Client.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

IFMessager::IFMessager()
{
	using namespace ftxui;

	Component textArea = Input(
		&chatMessage.str,
		"[enter] to send");
	
	//absolutely no automatic user id. high trust gameplay(trust that it wont crash)

	textArea |= CatchEvent([&](Event e) -> bool { 
		if (e.is_mouse()) return false;

		if (e == Event::Return) {
			using namespace Events;

			ClientEvent::observer.invokeEvent<ResolveableResponder>(
				ClientEvent::CLIENT_EVENT::ADD_RESOLVEABLE_RESPONDER,
				[=](Client& c) -> bool {

					ClientEvent::observer.invokeEvent<std::string>(
						ClientEvent::CLIENT_EVENT::EVENT_MESSAGE,
						chatMessage.str
					);

					static Message msg{ .header = {.id = NetMsgType::OtherMsg} };

					chatMessage.packMessage(msg);
					{//adding message meta
						OtherMsg on{.type = OtherMsg::TYPE::CHAT_MSG };
						msg << on;
					}
					
					chatMessage.str.clear();

					c.Send(msg);

					return true;
				});
		}

		return e.is_character() && chatMessage.str.size() > OTHERMSG::ChatMsg::MAX_LEN;
	});

	Add(textArea);
}
