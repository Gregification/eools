#include "Client.hpp"

void Client::run(ScreenInteractive& screen) {
	//update state
	{
		ConnectionStatus stat = ConnectionStatus();
		stat.isQueue = true;

		net::message<NetMsgType> msg;
		msg.header.id = NetMsgType::ConnectionStatus;
		msg << stat;
	}

	auto canvas_element = canvas([](Canvas& c) {
			c.DrawPointLine(1, 1, c.width() - 1, c.height() - 1, Color::Red);
			c.DrawBlock(0, 0, true, Color::Green);
		});
	auto flexible_canvas_element = canvas_element | flex;

	screen.Loop(Renderer([&] {
		if (!isConnected())
			return text("no connection");
		return vbox({
			flexible_canvas_element
			});
	}));
}

void Client::OnMessage(net::message<NetMsgType> msg) {
	switch (msg.header.id) {
		case NetMsgType::Ping: {
			Ping ping = Ping();
			msg >> ping;

			if (ping.isComplete()) {
				m_connection->pingTime = ping.getTime();
				break;
			}

			ping.tag();

			msg << ping;
			Send(msg);
		}break;
		case NetMsgType::IDPartition: {
			IDPartition idp = IDPartition();
			msg >> idp;
			if(!idp.isBad())
				LOCAL_PARITION = idp;
		}break;
	}
}