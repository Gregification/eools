#pragma once 

#include "../NormalCurve.hpp"

/**
* epic to the curvth order.
* chat, am i real?
*/
struct LinearCurve : public virtual NormalCurve {
	// Inherited via ScalerCurve
	void packMessage(Message&, MsgDiffType) override;
	void unpackMessage(Message&, MsgDiffType) override;
	float evaluate(const float&) const override;
	bool isFunction() const override;
};
