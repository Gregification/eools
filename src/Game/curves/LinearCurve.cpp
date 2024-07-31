#include "LinearCurve.hpp"

void LinearCurve::packMessage(Message&, MsgDiffType)
{
}

void LinearCurve::unpackMessage(Message&, MsgDiffType)
{
}

float LinearCurve::evaluate(const float& r) const
{
	return r;
}

bool LinearCurve::isFunction() const
{
	return true;
}
