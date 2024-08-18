#pragma once

namespace Power {
	class Generator;
}

#include "../NormalCurve.hpp"
#include "Loads.hpp"

#include "PowerBank.hpp"
#include "../../NetMessageType.hpp"
//planned this to be more intricate but didnt work out.
//	what remains is eh

namespace Power {
	/**
	* generates stuff and consumes stuff at the same time
	*
	* TODO: actually consume stuff. is waiting on item system
	*/
	class Generator : public Packable {
		friend class IFHelm;

		//live
		float throttle_n = 0;
		float throttle_n_c;

		std::unique_ptr<NormalCurve> gen_g;
		std::unique_ptr<NormalCurve> con_g;

		PowerUnit maxGenerate = 0;
		PowerUnit maxConsumption = 0;

		//cached gradiant output
		float c_gen_n = 0;
		float c_con_n = 0;
	public:

		Loads::LoadUnit fuel;

		Generator();

		void update(const float& dt, PowerBank&);

		const float& getThrottlePrecentage() const;
		void setThrottlePrecentage(float);

		const NormalCurve& getGenerationGradiant() const;
		const NormalCurve& getConsumptionGradiant() const;
		void setGenerationGradiant(std::unique_ptr<NormalCurve>);
		void setConsumptionGradiant(std::unique_ptr<NormalCurve>);

		const PowerUnit& getMaxGenerate() const;
		void setMaxGenerate(PowerUnit);
		const PowerUnit& getMaxConsumption() const;
		void setMaxConsumption(PowerUnit);

		virtual void packMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
		virtual void unpackMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
	};

}
