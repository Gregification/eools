#pragma once

#include "../GameStructs.hpp"
#include "../NetMessageType.hpp"
#include "../factory/Factory.hpp"

/**
* abstraction for curves.
* 
* packs all inherited data.
* 
* @return a scaler. not guarenteed to be normalized
*/
struct NormalCurve : public virtual Messageable, public Factory::FactoryInstable<NormalCurve, NormalCurve> {
	/**
	* evaluate curve at percentage of completion.
	* note that precentage is not necessarly relative to distance
	*/
	virtual float evaluate(const float&) const = 0;

	/**
	* @return true if the graph passes the horizontal line test
	*/
	virtual bool isFunction() const = 0;

	/**
	* brute force finder of best returns ratio for a curve
	* @param probes, the number of probes used
	* @return the R value where o/i is the highest
	*/
	static float findMaxEfficiency(const int& probes, const float& r_min, const float& r_max, const NormalCurve& curve);
	/**
	* brute force finder of worst returns ratio for a curve.
	* @param probes, the number of probes used
	* @return the R value where o/i is the lowest
	*/
	static float findMinEfficiency(const int& probes, const float& r_min, const float& r_max, const NormalCurve& curve);
};
