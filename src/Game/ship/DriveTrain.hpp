#pragma once

#include "Curves/IOPair.hpp"

class DriveTrain : public virtual Messageable {
public:

	void packMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
	void unpackMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
	
};
