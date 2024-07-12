#pragma once

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <unordered_map>
#include <map>
#include <unordered_set>

//should have added this earlier, most of the code dosent use it but anything new should
class GameObject;
typedef std::shared_ptr<GameObject> GameObjPtr;

#include "Game_common.hpp"
#include "NetMessageType.hpp"
#include "GameStructs.hpp"
#include "Body.hpp"
#include "IdGen.hpp"

using namespace ftxui;
using namespace gs;

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
	void setId(Instance_Id newId);

	Instance_Id inline id() const { return _instId; }

	std::string getDisplayName() const { return _displayName; }

	void addSynchronizationTarget(SyncTarget);

	SyncCollection getSynchronizationTargets();

	//as of now its all cpu based(cringe) but the plans to split off the heavier work 
	//	to the gpu eventually
	//https://stackoverflow.com/questions/34447682/what-is-the-difference-between-update-fixedupdate-in-unity
	virtual void Update(float);
	virtual void FixedUpdate(float);

	virtual void packMessage(Message&, MsgDiffType = MsgDiff_EVERYTHING);
	virtual void unpackMessage(Message&, MsgDiffType = MsgDiff_EVERYTHING);

	virtual void Draw(ftxui::Canvas&, gs::Transformation_2D&) const;

	virtual std::string GetDescription() const;


	virtual Class_Id GetClassId() const;

protected:
	Instance_Id _instId;
	std::string _displayName;

	SyncCollection _syncCollection;
};
