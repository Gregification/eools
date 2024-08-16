#include "ICGridLine.hpp"

Vec2 ICGridLine::getPoint(Camera& c, const Vec2_i& p) const
{
    return c.screenToGrid(p);
}
