#pragma once

#include "NormalCurve.hpp"

//pipline of curves with some extra control options
struct CurvePipe : public virtual Messageable {

	std::vector<NormalCurve> stages;

	/** r clamped to this range before input to each stage */
	float
		r_max = 1,
		r_min = 0;

	float passthough(float r) const;

	// Inherited via Messageable
	void packMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
	void unpackMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
};