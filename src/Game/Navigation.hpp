#pragma once

#include "../GameStructs.hpp"
#include "../GameObject.hpp"

namespace Navigation {
	using namespace gs;

	//dont see another way ot do this that isnt repeating the name 3 times
	//this way of having navigation be seperate from transform stops the 
	//		implimenetaiton from caching anything but eh, skill issue?

	enum TRAVEL_STATE : int8_t {
		_ALIGN,
		_ALIGN_TO,
		_APPROACH,
		_MAINTAIN_DISTANCE,
		_NONE,
		_ORBIT,
	};

	struct ALIGN {
		float targetRot;
	};

	struct ALIGN_TO {
		Instance_Id targetId;
		std::weak_ptr<GameObject> targetObj;
	};

	struct APPROACH {
		Vec2 point;
	};

	struct MAINTAIN_DISTANCE {
		Vec2 point;
		Unit distance;
	};

	struct ORBIT {
		Vec2 center;
		Unit radius;
	};

	//variant doesn't have a assured layout
	//unions do (well as much as the compiler wants you to think)
	union DestinationInfo {
		ALIGN				align;
		//ALIGN_TO			align_to;
		APPROACH			approach;
		//MAINTAIN_DISTANCE	maintain_distance;
		//ORBIT				orbit;
	};
	static_assert(std::is_standard_layout<DestinationInfo>::value);

	struct Navigator {
		TRAVEL_STATE travelState;
		DestinationInfo destInfo;

		void navigate(gs::Transform&);
	};
	static_assert(std::is_standard_layout<Navigator>::value);
};