#pragma once

#include "../GameStructs.hpp"
#include "../NetMessageType.hpp"

/**
* abstraction for curves.
* @return a scaler. not guarenteed to be normalized
*/
struct ScalerCurve : public virtual Messageable {
	/**
	* evaluate curve at percentage of completion.
	* note that precentage is not necessarly relative to distance
	*/
	virtual float evaluate(const float&) const = 0;

	/**
	* @return true if the graph passes the horizontal line test
	*/
	virtual bool isFunction() const = 0;
};
