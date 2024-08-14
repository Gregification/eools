#include "ConstCurve.hpp"

void ConstCurve::packMessage(Message& msg, MsgDiffType)
{
	msg << value;
}

void ConstCurve::unpackMessage(Message& msg, MsgDiffType)
{
	msg >> value;
}

float ConstCurve::evaluate(const float&) const
{
	return value;
}

bool ConstCurve::isFunction() const
{
	return true;
}
