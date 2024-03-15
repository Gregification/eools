#pragma once

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <eol_net.hpp>

#include "GameStructs.hpp"

using namespace gs;
using namespace ftxui;

class GameObject {
	public:
		GameObject(id_t id) : transform(id) {};

		virtual void Draw(Canvas& c, Dimensions zoom)	const {
			c.DrawBlock(transform.position.x, transform.position.y, true);
		};
		
		//pack the current class, then pack the super class
		virtual void packMessage(net::message<NetMsgType>& msg) const { 
			msg << transform;
		}

		//unpack the current class, then unpack the super class
		virtual void unpackMessage(net::message<NetMsgType>& msg) {
			msg >> transform;
		}

	public:
		Transform transform;
};