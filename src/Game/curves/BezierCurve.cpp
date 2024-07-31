#include "BezierCurve.hpp" 

const std::array<float, BC_N> BezierCurve::btkvw = {
		ct_binomial_coef(BC_N, 0),
		ct_binomial_coef(BC_N, 1),
		ct_binomial_coef(BC_N, 2),
		ct_binomial_coef(BC_N, 3)
};

float BezierCurve::evaluate(const float& r) const
{
	//using the formula from the desmos example mentioned in this structs description

	const float dr = 1 - r;
	
	//r power
	float rp = 1;

	float numerator = 0;
	float denominator = 0;

	for (int k = 0; k < BC_N; k++) {
		const float allbutV = 
			btkvw[k]					//binomial(N,K)
			* std::powf(dr, BC_N - k)	//(1-t)^(N-K)
			* rp						//t^k
			* weights[k];				//w
		rp *= r;

		numerator	+= allbutV * points[k];
		denominator += allbutV;

	}

	return numerator / denominator;
}

bool BezierCurve::isFunction() const
{
	return false;
}

void BezierCurve::packMessage(Message& msg, MsgDiffType)
{
	msg << points;
	msg << weights;
}

void BezierCurve::unpackMessage(Message& msg, MsgDiffType)
{
	msg >> weights;
	msg >> points;
}
