#pragma once

#include "../NormalCurve.hpp"
#include "../Curves/LinearCurve.hpp"

namespace Loads {
	struct LoadRequest {
		Unit draw, min_draw, received;
	};
	typedef std::shared_ptr<LoadRequest> Load;

	class LoadHandler_IOPair {
		float i_r;
		float o_r;

		Unit draw;
		Unit max_draw;

		std::vector<std::weak_ptr<LoadRequest>> loads;

	public:
		/**
		* not a pipeline.
		* given a single R value
		*	- 'c_input' represents 
		*/
		std::shared_ptr<NormalCurve> c_input  = std::make_shared<LinearCurve>();
		std::shared_ptr<NormalCurve> c_output = std::make_shared<LinearCurve>();
		
		const Unit& getInputR() const;
		void setInputR(const float&);

		const Unit& getMaxDraw() const;
		void setMaxDraw(const Unit&);

		void addLoad(Load);

		std::vector<std::weak_ptr<LoadRequest>>& getLoads();
		void removeLoad(Load);

		void recalLoads();
		void recalOutput();
	};
};
