#pragma once

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <unordered_map>
#include <map>

#include "NetMessageType.hpp"
#include "GameStructs.hpp"
#include "Body.hpp"
#include "IdGen.hpp"
#include "GameObjectFactory.hpp"

using namespace gs;
using namespace ftxui;

/*
* physics & game-stat related details are in the Body subclass
*/
class GameObject : public IdGen<GameObject> , virtual public Body {
public:
	GameObject(Instance_Id id = BAD_ID) :
		Body()
	{
		setId(id);
	}
	virtual ~GameObject() = default;

public:
	void setDisplayName(std::string);
	virtual std::string getDisplayId();
	virtual std::string getDisplayName();

	void setId(Instance_Id newId);
	Instance_Id id() { return instId; }

	virtual void Update(float dt) {}

	virtual void FixedUpdate(float dt) {
		transform.FixedUpdate(dt);
	}

	virtual void Draw(Canvas&, Transformation_2D&) const;

	//PACK CURRENT CLASS FIRST, SUPER CALSS LAST
	virtual void packMessage(net::message<NetMsgType>& msg);

	//UNPACK SUPER CLASS FIRST
	virtual void unpackMessage(net::message<NetMsgType>& msg);

	virtual std::string GetDescription() const;

	virtual bool NeedsNetUpdate();

	virtual Class_Id GetClassId() const;

	/*virtual inline GameObjectFactory getGOF()	const = 0;
	virtual inline ID getID()					 = 0;*/

private:
	Instance_Id instId;
	std::string displayName;
	std::string displayId;
};
