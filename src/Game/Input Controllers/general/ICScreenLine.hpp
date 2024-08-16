#pragma once

#include "ICLine.hpp"

class ICScreenLine : public ICLine {
	Vec2 getPoint(Camera&, const Vec2_i& screenPoint) const override;
};
