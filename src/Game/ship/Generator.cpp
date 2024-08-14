#include <algorithm>

#include "Generator.hpp"

#include "../curves/LinearCurve.hpp"

using namespace Power;

enum class DIFF : MsgDiffType {
	throttle_n = DEFAULT_MsgDiff_EVERYTHING+1,
	gen_gradient,
	con_gradient,
	maxGen,
	maxCon,
	fuel
};

Generator::Generator()
{
	gen_g = std::make_unique<LinearCurve>();
	con_g = std::make_unique<LinearCurve>();
}

void Generator::update(const float& dt, PowerBank& pb)
{
	if (!pb.isFull()) {
		auto gen = c_gen_n * maxGenerate * dt;
		auto con = c_con_n * maxConsumption * dt;

		//lerp the difference

		if (con > fuel) {
			gen *= fuel / con;
			con = fuel;
		}

		pb.setValue(pb.getValue() + gen);

		if (pb.getValue() > pb.getCapacity()) {
			con *= 1 - ((pb.getValue() - pb.getCapacity()) / maxGenerate);

			pb.setValue(pb.getCapacity());
		}

		fuel -= con;
		fuel = std::max(Loads::LoadUnit{0}, fuel);
	}
}

const float& Generator::getThrottlePrecentage() const
{
	return throttle_n;
}

void Generator::setThrottlePrecentage(float n)
{
	throttle_n = std::min(1.0f, std::max(0.0f, n));
	addPackageTarget(DIFF::throttle_n);

	c_gen_n = gen_g->evaluate(throttle_n);
	c_con_n = con_g->evaluate(throttle_n);
}

const NormalCurve& Generator::getGenerationGradiant() const
{
	return *gen_g;
}

const NormalCurve& Generator::getConsumptionGradiant() const
{
	return *con_g;
}

void Generator::setGenerationGradiant(std::unique_ptr<NormalCurve> p)
{
	if (p) {
		gen_g = std::move(p);
		addPackageTarget(DIFF::gen_gradient);
	}
}

void Generator::setConsumptionGradiant(std::unique_ptr<NormalCurve> p)
{
	if (p) {
		con_g = std::move(p);
		addPackageTarget(DIFF::con_gradient);
	}
}

const PowerUnit& Power::Generator::getMaxGenerate() const
{
	return maxGenerate;
}

void Power::Generator::setMaxGenerate(PowerUnit n)
{
	maxGenerate = n;
	addPackageTarget(DIFF::maxGen);
}

const PowerUnit& Power::Generator::getMaxConsumption() const
{
	return maxConsumption;
}

void Power::Generator::setMaxConsumption(PowerUnit n)
{
	maxConsumption = n;
	addPackageTarget(DIFF::maxCon);
}

void Generator::packMessage(Message& msg, MsgDiffType t)
{
	//i really should make this a macro
	switch (t)
	{
	case static_cast<MsgDiffType>(DIFF::throttle_n):		
		msg << throttle_n;		break;

	case static_cast<MsgDiffType>(DIFF::maxGen):		
		msg << maxGenerate;		break;

	case static_cast<MsgDiffType>(DIFF::maxCon):		
		msg << maxConsumption;	break;

	case static_cast<MsgDiffType>(DIFF::fuel):			
		msg << fuel;			break;

	case static_cast<MsgDiffType>(DIFF::gen_gradient):	
		NormalCurve::factory.PackAs(msg, gen_g.get());	break;

	case static_cast<MsgDiffType>(DIFF::con_gradient):	
		NormalCurve::factory.PackAs(msg, con_g.get());	break;

	case DEFAULT_MsgDiff_EVERYTHING:
	default:
		
		NormalCurve::factory.PackAs(msg, gen_g.get());
		NormalCurve::factory.PackAs(msg, con_g.get());
		msg << throttle_n;
		msg << maxGenerate;
		msg << maxConsumption;
		msg << fuel;

		break;
	}
}

void Generator::unpackMessage(Message& msg, MsgDiffType t)
{
	switch (t)
	{
	case static_cast<MsgDiffType>(DIFF::throttle_n):
		msg >> throttle_n;		break;

	case static_cast<MsgDiffType>(DIFF::maxGen):
		msg >> maxGenerate;		break;

	case static_cast<MsgDiffType>(DIFF::maxCon):
		msg >> maxConsumption;	break;

	case static_cast<MsgDiffType>(DIFF::fuel):
		msg >> fuel;			break;

	case static_cast<MsgDiffType>(DIFF::gen_gradient):
		NormalCurve::factory.UnpackAs(msg, gen_g.get());	break;

	case static_cast<MsgDiffType>(DIFF::con_gradient):
		NormalCurve::factory.UnpackAs(msg, con_g.get());	break;

	case DEFAULT_MsgDiff_EVERYTHING:
	default:
		msg << fuel;
		msg << maxConsumption;
		msg << maxGenerate;
		msg << throttle_n;
		NormalCurve::factory.UnpackAs(msg, con_g.get());
		NormalCurve::factory.UnpackAs(msg, gen_g.get());
		break;
	}
}

