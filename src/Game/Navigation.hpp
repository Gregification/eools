#pragma once

#include "../GameStructs.hpp"
#include "../GameObject.hpp"
#include "../GameObjects/Grid.hpp"

namespace Navigation {
	using namespace gs;

	//dont see another way ot do this that isnt repeating the name 3 times
	//this way of having navigation be seperate from transform stops the 
	//		implimenetaiton from caching anything but eh, skill issue?

	enum class TRAVEL_STATE : int8_t {
		ALIGN,
		ALIGN_TO,
		APPROACH,
		MAINTAIN_DISTANCE,
		NONE,
		ORBIT,
	};

	struct ALIGN {
		float targetRot;
	};

	struct ALIGN_TO {
		ID target;
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
		ALIGN_TO			align_to;
		APPROACH			approach;
		MAINTAIN_DISTANCE	maintain_distance;
		ORBIT				orbit;

		//required for non trivial constructors from Vec2_
		DestinationInfo() { memset(this, 0, sizeof(DestinationInfo)); }
	};
	static_assert(std::is_standard_layout<DestinationInfo>::value);

	struct NavInfo {
		TRAVEL_STATE travelState;
		DestinationInfo destInfo;
	};
	static_assert(std::is_standard_layout<NavInfo>::value);
};