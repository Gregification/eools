#include "CurvePipe.hpp"

float CurvePipe::passthough(float r) const
{
	for (auto&& v : stages) {
		r = std::max(0.0f, std::min(r, 1.0f));
		r = v->evaluate(r);
	}

	return r;
}

void CurvePipe::packMessage(Message& msg, MsgDiffType)
{
	packArray<NormalCurve>(msg, stages, [](Message& msg,  NormalCurve* nc) {
		Class_Id cid = NormalCurve::factory.GetClassId(nc);
		NormalCurve::factory.PackAs(cid, msg, nc);
		nc->packMessage(msg, DEFAULT_MsgDiff_EVERYTHING);
		msg << cid;
	});
}

void CurvePipe::unpackMessage(Message& msg, MsgDiffType)
{
	stages.clear();
	unpackArray<NormalCurve>(msg, stages, [](Message& m) ->std::unique_ptr<NormalCurve> {
		Class_Id cid;
		m >> cid;

		auto p = NormalCurve::factory.GetInstance(cid);
		p->unpackMessage(m);

		return p;
	});
}
