#include "StepCurve.hpp"

void StepCurve::packMessage(Message& msg, MsgDiffType)
{
	msg << stepUp;
	msg << numSteps;
}

void StepCurve::unpackMessage(Message& msg, MsgDiffType)
{
	msg >> numSteps;
	msg >> stepUp;
}

float StepCurve::evaluate(const float& r) const
{
	float v = static_cast<int>(r * numSteps) / numSteps;

	if (stepUp)
		return 1 - v;
	else
		return v;
}

bool StepCurve::isFunction() const
{
	return false;
}
