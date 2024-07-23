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
class GameObject : public IdGen<GameObject> , virtual public Body, virtual public Messageable {
public:
	GameObject(Instance_Id id = BAD_ID) :
		Body()
	{
		setId(id);
		_updateTimes.lastFixedUpdate = _updateTimes.lastUpdate = GetTime();
	}

	UpdateTime _updateTimes;

public:
	void setId(Instance_Id newId);

	Instance_Id inline id() const { return _instId; }

	std::string getDisplayName() const { return _displayName; }

	const SyncCollection& getSynchronizationTargets();
	void clearSynchronizationTargets();

	//as of now its all cpu based(cringe) but the plans to split off physics 
	// to the gpu eventually
	//https://stackoverflow.com/questions/34447682/what-is-the-difference-between-update-fixedupdate-in-unity
	virtual void Update(float);
	virtual void FixedUpdate(float);

	virtual void packMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
	virtual void unpackMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;

	virtual void Draw(ftxui::Canvas&, gs::Transformation_2D) const;

	virtual std::string GetDescription() const;

	virtual Class_Id GetClassId() const;

	//javadoc ftw, didnt realize it works on non java things
	/**
	* checks if the point relative to the GO is within the GO.
	* does not check AABB or any higherlevel generalizations
	* 
	* @param point Point relative to gameobject
	* @return true if point is within the GO's body
	*/	
	virtual bool ContainsPoint(const Vec2_f& point) const;

protected:
	void addSynchronizationTarget(SyncTarget);

	Instance_Id _instId;
	std::string _displayName = "default go dispaly name";

	SyncCollection _syncCollection;
};
