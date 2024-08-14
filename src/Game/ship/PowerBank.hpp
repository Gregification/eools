#pragma once

#include "Loads.hpp"
#include "../../NetMessageType.hpp"

namespace Power {
	struct PowerBank : public Packable {
		bool isFull() const;

		const PowerUnit& getCapacity() const;
		const PowerUnit& getValue() const;
		void setCapacity(PowerUnit);
		void setValue(PowerUnit);

		// Inherited via Packable
		void packMessage(Message&, MsgDiffType) override;
		void unpackMessage(Message&, MsgDiffType) override;
		
	private:
		//synched
		PowerUnit capacity = 0;
		PowerUnit value = 0;
	};
}