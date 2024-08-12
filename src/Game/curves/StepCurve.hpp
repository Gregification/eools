#pragma once

#include "../NormalCurve.hpp"
#include "../../factory/Factory.hpp"

/**
* a linear steped funciton. 
* not guarenteed out put of 1@1 .
* guarenteed output of 0@0 .
* 
* see visualization 
*	- desmos
*	- stepUp equation : y=\frac{\operatorname{floor}\left(x\cdot s\right)}{s}
*	- stepDown equation : y=1-\frac{\operatorname{floor}\left(x\cdot s\right)}{s}
*	- s : slider for # of steps
*/
struct StepCurve : public virtual NormalCurve, public Factory::FactoryInstable<NormalCurve, StepCurve> {
	bool stepUp = true;
	float numSteps = 2.01f;//a 2 step but does 1@1

	// Inherited via ScalerCurve
	void packMessage(Message&, MsgDiffType) override;
	void unpackMessage(Message&, MsgDiffType) override;
	float evaluate(const float&) const override;
	bool isFunction() const override;
};
