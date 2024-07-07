#pragma once

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <unordered_map>
#include <map>

#include "Game_common.hpp"
#include "NetMessageType.hpp"
#include "GameStructs.hpp"
#include "Body.hpp"
#include "IdGen.hpp"

using namespace ftxui;
using namespace gs;

//should have added this earlier, most of the code dosent use it but anything new should
class GameObject;
typedef std::shared_ptr<GameObject> GameObjPtr;

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
	Instance_Id inline id() const { return instId; }

	//as of now its all cpu based(cringe) but the plans to split off the heavier work 
	//	to the gpu eventually
	//https://stackoverflow.com/questions/34447682/what-is-the-difference-between-update-fixedupdate-in-unity
	virtual void Update(float);
	virtual void FixedUpdate(float);

	virtual void Draw(ftxui::Canvas&, gs::Transformation_2D&) const;

	//PACK CURRENT CLASS FIRST, SUPER CALSS LAST
	virtual void packMessage(Message& msg);

	//UNPACK SUPER CLASS FIRST
	virtual void unpackMessage(Message& msg);

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
