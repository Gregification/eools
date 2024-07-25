#pragma once

#include <unordered_map>
#include <functional>

#include "../GameStructs.hpp"
#include "../GameObject.hpp"

class Ship;

namespace Navigation {
	#define Nav_Base_OverrideFuncs  \
		void packMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override; \
		void unpackMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override; \
		void refresh() override; \
		void nav_update(float) override; \
		constexpr TRAVEL_STATE::_enumerated getTravelState() override;

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

	//structs because it implies these arnt unique classes or what ever (also convient)
	/**
	* navigation interface. this is intended to navigate a single 'Ship', and lets 
	*	children cache values to allow for more complex nav processes. this means 
	*	this that its probebly faster to use a single 'NavBase' per 'Ship' rather 
	*	than a single 'NavBase' directing mutiple 'Ship's.
	*/
	struct NavBase : Messageable {
		std::shared_ptr<Ship> getShip() { return ship.lock(); }
		void setShip(std::weak_ptr<Ship> s) { ship = s; }

		/**
		* resets what ever cache variables there are
		*/
		virtual void refresh() = 0;

		//some concerns on how to effeciently access the target ship, for now just 
		//	brute force it by getting a shared_ptr from the weak_ptr each update
		/**
		* updates the selected ship for the nav pattern
		*/
		virtual void nav_update(float) = 0;

		virtual constexpr TRAVEL_STATE::_enumerated getTravelState() = 0;

	protected:
		/** thing to navigate*/
		std::weak_ptr<Ship> ship;
	};
	
	struct NavInfo : Messageable {
		std::unique_ptr<NavBase> navPattern;

		void packMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
		void unpackMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
	};


	struct ALIGN : NavBase {
		Nav_Base_OverrideFuncs

		float targetRot;
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