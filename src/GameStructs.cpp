#include "GameStructs.hpp"

using namespace gs;

//really should move some stuff out of the header file

void Transform::applyAccele(float magnitude, float radian) {
	acceleration += Vec2_f::Rot({ magnitude }, {0}, radian);
}