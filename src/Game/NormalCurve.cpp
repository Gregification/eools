#include "NormalCurve.hpp"

//this dosent quite work at low bounds but eh
float NormalCurve::findMaxEfficiency(const int& probes, const float& min, const float& max, const NormalCurve& c)
{
    //nothing fancy

    float d = (max - min) / probes;

    float e_at_r = c.evaluate(min) / min;
    float r = min;
    for (float x = min; x <= max; x += d) {

        float e_at_x = c.evaluate(x) / x;

        if (e_at_x > e_at_r) {
            e_at_r = e_at_x;
            r = x;
        }
    }

    return r;
}

float NormalCurve::findMinEfficiency(const int& probes, const float& min, const float& max, const NormalCurve& c)
{
    float d = (max - min) / probes;

    float e_at_r = c.evaluate(min) / min;
    float r = min;
    for (float x = min; x <= max; x += d) {

        float e_at_x = c.evaluate(x) / x;

        if (e_at_x < e_at_r) {
            e_at_r = e_at_x;
            r = x;
        }
    }

    return r;
}
