#include "CurvePipe.hpp"

float CurvePipe::passthough(float r) const
{
	for (const auto& v : stages) {
		r = std::max(r_min, std::min(r, r_max));
		r = v.evaluate(r);
	}

	return r;
}

void CurvePipe::packMessage(Message& msg, MsgDiffType)
{
	msg >> r_max;
	msg >> r_min;

	//packArray(msg, stages, [](Message& m, NormalCurve& n) {n.packMessage(m); });
}

void CurvePipe::unpackMessage(Message& msg, MsgDiffType)
{
	//packArray(msg, stages, [](Message& m, NormalCurve& n) {n.unpackMessage(m); });

	msg >> r_min;
	msg >> r_max;
}
