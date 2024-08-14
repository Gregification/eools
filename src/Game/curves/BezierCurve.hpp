#pragma once

#include <array>

#include "../NormalCurve.hpp"
#include "../../factory/Factory.hpp"

/**
* 4th order rational bezier curve.
* shouldnt need anything higher, if you do, the main algorithms in the 'evaluate(...)' function
* just make the weights 0 to get a lower order.
* 
* see for visualization
*	- n-th rationalized (very cool) https://www.desmos.com/calculator/kiqc4acwwk
*		- c&p this into a new formula slot for line visualization
*				\left(\left(\frac{\left(b\left(t,0,x_{1},w_{1}\right)+b\left(t,1,x_{2},w_{2}\right)+b\left(t,2,x_{3},w_{3}\right)+b\left(t,3,x_{4},w_{4}\right)+b\left(t,4,x_{5},w_{5}\right)+b\left(t,5,x_{6},w_{6}\right)\right)}{\left(b\left(t,0,1,w_{1}\right)+b\left(t,1,1,w_{2}\right)+b\left(t,2,1,w_{3}\right)+b\left(t,3,1,w_{4}\right)+b\left(t,4,1,w_{5}\right)+b\left(t,5,1,w_{6}\right)\right)},\ \frac{\left(b\left(t,0,y_{1},w_{1}\right)+b\left(t,1,y_{2},w_{2}\right)+b\left(t,2,y_{3},w_{3}\right)+b\left(t,3,y_{4},w_{4}\right)+b\left(t,4,y_{5},w_{5}\right)+b\left(t,5,y_{6},w_{6}\right)\right)}{\left(b\left(t,0,1,w_{1}\right)+b\left(t,1,1,w_{2}\right)+b\left(t,2,1,w_{3}\right)+b\left(t,3,1,w_{4}\right)+b\left(t,4,1,w_{5}\right)+b\left(t,5,1,w_{6}\right)\right)}\right)\right)
*		- set N to 3
*		- red -> orange -> green -> blue
*		- suggested layout, red@(0,0), blue@(1,1). do what ever with orange and green
*		- output is y axis
*/
struct BezierCurve : public NormalCurve, public Factory::FactoryInstable<NormalCurve, BezierCurve> {
	#define BC_N 4

	std::array<float, BC_N> points {0,	0.25f,	0.75f,	1};//defaults to linear
	std::array<float, BC_N> weights{1,		1,		1,	1};

	// Inherited via ScalerCurve
	float evaluate(const float&) const override;
	bool isFunction() const override;
	void packMessage(Message&, MsgDiffType) override;
	void unpackMessage(Message&, MsgDiffType) override;

private:
	static const std::array<float, BC_N> btkvw;
};
