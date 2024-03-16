#pragma once

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

#include "NetMessageType.h"
#include "GameStructs.hpp"
#include "Game_common.hpp"

using namespace gs;
using namespace ftxui;

class GameObject {
	public:
		GameObject(id_t id) : transform(id) {};

		virtual void Update(time_t dealtaTime, time_t currTime) {

		}

		virtual void Draw(Canvas& c, const Vec2& offset, float scale)	const {
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

		virtual std::string GetDescription() const {
			return "gameobject description";
		}

		virtual bool NeedNetUpdate() {
			return needNetUpdate;
		}

	public:
		bool needNetUpdate = true;
		Transform transform;
};

typedef struct GId_pair : std::pair<id_t, std::shared_ptr<GameObject>> {
	static const id_t* util_packArray(const GId_pair* arr, size_t i) {
		return &(arr[i].first);
	}

	static GId_pair util_unpackArray(net::message<NetMsgType>& msg) {
		GId_pair ret = {};
		msg >> ret.first;
		ret.second = NULL;

		return ret;
	}
	
	friend bool operator == (const GId_pair& a, const GId_pair& b) {
		return a.first == b.first && a.first != NAN;
	}
} GId_pair;