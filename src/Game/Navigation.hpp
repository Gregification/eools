#pragma once

#include "../better-enums/enum.h"
#include "../GameStructs.hpp"
#include "../GameObjects/Ship.hpp"
#include "InputControl.hpp"

namespace Navigation {
	#define Nav_Base_OverrideFuncs  \
		void packMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override; \
		void unpackMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override; \
		void reset() override; \
		void nav_update(float, Ship&) override; \
		TRAVEL_STATE::_enumerated getTravelState() const override;

	using namespace gs;

	//dont see another way ot do this that isnt repeating the name 3 times
	//this way of having navigation be seperate from transform stops the 
	//		implimenetaiton from caching anything but eh, skill issue?
	BETTER_ENUM(TRAVEL_STATE, int8_t,
		NONE,
		ALIGN,
		ALIGN_TO,
		APPROACH,
		MAINTAIN_DISTANCE,
		ORBIT
	);

	struct NavBase;

	extern std::unordered_map<
		TRAVEL_STATE::_enumerated,
		std::function<std::unique_ptr<NavBase>()>
	> stateToNavigator;

	extern std::unordered_map<
		TRAVEL_STATE::_enumerated,
		std::function<InputController()>
	> stateToNavigatorIC;

	//structs because it implies these arnt unique classes or what ever (also convient)
	/**
	* navigation interface. this is intended to navigate a single 'Ship', and lets 
	*	children cache values to allow for more complex nav processes. this means 
	*	this that its probebly faster to use a single 'NavBase' per 'Ship' rather 
	*	than a single 'NavBase' directing mutiple 'Ship's.
	*/
	struct NavBase : Messageable {
		/**
		* resets what ever cache variables there are
		*/
		virtual void reset();

		//some concerns on how to effeciently access the target ship, for now just 
		//	brute force it by getting a shared_ptr from the weak_ptr each update
		/**
		* updates the selected ship for the nav pattern
		*/
		virtual void nav_update(float, Ship&);

		virtual TRAVEL_STATE::_enumerated getTravelState() const;
	};

	struct ALIGN : NavBase {
		Nav_Base_OverrideFuncs

		float targetRot;
		float rate;
	};

	struct ALIGN_TO : NavBase {
		Nav_Base_OverrideFuncs

		//this should really be just instance id (for now) and we assume its in the current grid
		// but lets be optiomiztic that thi sproject would get far enough along 
		// that can actually be implimented! :) (i dont think so :( )
		ID target;

		std::weak_ptr<GameObject> targetPtr;
	};

	struct APPROACH : NavBase {
		Nav_Base_OverrideFuncs

		Vec2 point;
	};

	struct MAINTAIN_DISTANCE : NavBase {
		Nav_Base_OverrideFuncs

		Vec2 point;
		Unit distance;
	};

	struct ORBIT : NavBase {
		Nav_Base_OverrideFuncs

		Vec2 center;
		Unit radius;
	};
};
