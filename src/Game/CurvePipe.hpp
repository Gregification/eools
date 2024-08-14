#pragma once

#include "NormalCurve.hpp"

/**
* pipline of curves with some extra control options
* 
* this was made with 2 way synching so theres some overhead,
*	there are better alternatives if only 1 way synching is
*	needed.
*/
struct CurvePipe : public virtual Messageable {

	std::vector<std::unique_ptr<NormalCurve>> stages;

	float passthough(float r) const;

	// Inherited via Messageable
	void packMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
	void unpackMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
};