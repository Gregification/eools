#pragma once

#include "../NormalCurve.hpp"

/**
* evaluates to a constant number no matter the input
*/
struct ConstCurve : public NormalCurve, public Factory::FactoryInstable<NormalCurve, ConstCurve> {
	float value;

	ConstCurve(const float& v = 1) : value(v) {}

	void packMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
	void unpackMessage(Message&, MsgDiffType = DEFAULT_MsgDiff_EVERYTHING) override;
	float evaluate(const float&) const override;
	bool isFunction() const override;
};
