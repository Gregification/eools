#pragma once

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <unordered_map>
#include <map>

#include "NetMessageType.hpp"
#include "GameStructs.hpp"
#include "Game_common.hpp"
#include "Body.hpp"

class GameObjectFactory;

using namespace gs;
using namespace ftxui;

class GameObject : virtual public Body {
public:
	GameObject(id_t id) :
		Body(),
		id(id),
		needNetUpdate(false)
	{

	}
	virtual ~GameObject() = default;

public:
	id_t id;
	bool needNetUpdate;

public:
	virtual void Update(float dt) {}

	virtual void PhysUpdate(float dt) {
		transform.PhysUpdate(dt);
	}

	virtual void Draw(Canvas&, Transformation_2D&) const;

	//PACK CURRENT CLASS FIRST, SUPER CALSS LAST
	virtual void packMessage(net::message<NetMsgType>& msg) {
		msg << transform;
	}

	//UNPACK SUPER CLASS FIRST
	virtual void unpackMessage(net::message<NetMsgType>& msg) {
		msg >> transform;
	}

	virtual std::string GetDescription() const;

	virtual bool NeedNetUpdate();

	//GameObjectFactory stuff
	virtual GameObjectFactory getGOF()	const = 0;
	virtual ID getID()					const = 0;
};
