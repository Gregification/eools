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

/*
* networked game object.
* physics & game-stat related details are in the Body subclass
*/
class GameObject : virtual public Body {
public:
	GameObject(inst_id id) :
		Body(),
		needNetUpdate(false)
	{
		setId(id);
	}
	virtual ~GameObject() = default;

public:
	bool needNetUpdate;
	
	void setDisplayName(std::string);
	std::string getDisplayId();
	std::string getDisplayName();

	void setId(inst_id newId);
	inst_id id() { return instId; }

	virtual void Update(float dt) {}

	virtual void PhysUpdate(float dt) {
		transform.PhysUpdate(dt);
	}

	virtual void Draw(Canvas&, Transformation_2D&) const;

	//PACK CURRENT CLASS FIRST, SUPER CALSS LAST
	virtual void packMessage(net::message<NetMsgType>& msg) {
		msg << transform;
		msg << mass;

	}

	//UNPACK SUPER CLASS FIRST
	virtual void unpackMessage(net::message<NetMsgType>& msg) {
		msg >> mass;
		msg >> transform;
	}

	virtual std::string GetDescription() const;

	virtual bool NeedNetUpdate();

	//GameObjectFactory stuff
	virtual inline GameObjectFactory getGOF()	const = 0;
	virtual inline ID getID()					 = 0;

private:
	inst_id instId;
	std::string displayName;
	std::string displayId;
};
