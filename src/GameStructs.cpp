#include "GameStructs.hpp"

using namespace gs;

//really should move some stuff out of the header file

Transformation_2D Transformation_2D::identity   = { .mat = {{
        {{1,0,0}},	//double stuffed oreo
        {{0,1,0}},
        {{0,0,1}}	//Z = 1
    }}};

Transformation_2D Transformation_2D::clear  = { .mat = {{
        {{0,0,0}},
        {{0,0,0}},
        {{0,0,1}}	//for consistency
    }}};

Transformation_2D Transformation_2D::zero   = { .mat = {{
        {{0,0,0}},
        {{0,0,0}},
        {{0,0,0}}
    }}};

Arr3 Transformation_2D::mul(const Arr3& arr) const {
	// | A B C |   | X |   | X |
	// | D E F | * | Y | = | Y |
	// | G H I |   | Z |   | Z |
	//  [this]     [arr]   [out]
	Arr3 out;

	out[0] =
		PT(mat, 0, 0) * arr[0]  //ax
		+ PT(mat, 1, 0) * arr[1]  //by
		+ PT(mat, 2, 0) * arr[2]; //cz
	out[1] =
		PT(mat, 0, 1) * arr[0]  //dx
		+ PT(mat, 1, 1) * arr[1]  //ey
		+ PT(mat, 2, 1) * arr[2]; //fz
	out[2] =
		PT(mat, 0, 2) * arr[0]  //gx
		+ PT(mat, 1, 2) * arr[1]  //hy
		+ PT(mat, 2, 2) * arr[2]; //iz

	return out;
}

Mat3x3 Transformation_2D::mul(const Transformation_2D& o) {
	// | A B C |   | J K L |   | S T U |
	// | D E F | * | M N O | = | V W X |
	// | G H I |   | P Q R |   | Y Z $ |
	//  [this]      [other]		 [out]

	Mat3x3 out;

	Arr3 tmpthis;

	//assuming [y][x] setup
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			PT(out, x, y) = 
				  PT(mat, 0, y) * PT(o.mat, x, 0)
				+ PT(mat, 1, y) * PT(o.mat, x, 1)
				+ PT(mat, 2, y) * PT(o.mat, x, 2);
		}
	}

	return out;
}

void Transform::applyAccele(float magnitude, float radian) {
	acceleration += Vec2_f::Rot({ magnitude }, { 0 }, radian);
}

std::string gs::getPrettyString(float n) {
	static const int maxD = 3;
	static const int imaxD = 1 / maxD;

	//d : exact number of difits
	int d = static_cast<int>(std::log10f(std::abs(n)));

	//ad : number of steps, and a special case to have at least a 1's place
	int ad = d * imaxD * maxD - (d < 0) * maxD;

	//adjusts decimal to the step accounting that step != #digits
	n *= std::pow(10, -d + d - ad);

	//cant figure out how to insert maxD automatically
	return std::format("{:02d}e{:03.1f}", ad, n);
}

float Q_inverseSqrt(float number) {
	static_assert(std::numeric_limits<float>::is_iec559,
		"fast inverse square root requires IEEE-comliant 'float'");
	static_assert(sizeof(float) == sizeof(std::uint32_t),
		"fast inverse square root requires 'float' to be 32-bit");
	float x2 = number * 0.5F, y = number;
	std::uint32_t i;
	std::memcpy(&i, &y, sizeof(float));
	i = 0x5f3759df - (i >> 1);
	std::memcpy(&y, &i, sizeof(float));
	return y * (1.5F - (x2 * y * y));
}

//code converted from
//https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon
template<typename T>
bool DoVecsIntersect(
		const Vec2_T<T>& v1,
		const Vec2_T<T>& e1,
		const Vec2_T<T>& v2,
		const Vec2_T<T>& e2
) {
	float d1, d2;
	float a1, a2, b1, b2, c1, c2;

    // Convert vector 1 to a line (line 1) of infinite length.
    // We want the line in linear equation standard form: A*x + B*y + C = 0
    // See: http://en.wikipedia.org/wiki/Linear_equation
    a1 = e1.y - v1.y;
    b1 = v1.x - e1.x;
    c1 = (e1.x * v1.y) - (v1.x * e1.y);

    // Every point (x,y), that solves the equation above, is on the line,
    // every point that does not solve it, is not. The equation will have a
    // positive result if it is on one side of the line and a negative one 
    // if is on the other side of it. We insert (x1,y1) and (x2,y2) of vector
    // 2 into the equation above.
    d1 = (a1 * v2.x) + (b1 * v2.y) + c1;
    d2 = (a1 * e2.x) + (b1 * e2.y) + c1;

    // If d1 and d2 both have the same sign, they are both on the same side
    // of our line 1 and in that case no intersection is possible. Careful, 
    // 0 is a special case, that's why we don't test ">=" and "<=", 
    // but "<" and ">".
    if (d1 > 0 && d2 > 0) return false;
    if (d1 < 0 && d2 < 0) return false;

    // The fact that vector 2 intersected the infinite line 1 above doesn't 
    // mean it also intersects the vector 1. Vector 1 is only a subset of that
    // infinite line 1, so it may have intersected that line before the vector
    // started or after it ended. To know for sure, we have to repeat the
    // the same test the other way round. We start by calculating the 
    // infinite line 2 in linear equation standard form.
    a2 = e2.y - v2.y;
    b2 = v2.x - e2.x;
    c2 = (e2.x * v2.y) - (v2.x * e2.y);

    // Calculate d1 and d2 again, this time using points of vector 1.
    d1 = (a2 * v1.x) + (b2 * v1.y) + c2;
    d2 = (a2 * e1.x) + (b2 * e1.y) + c2;

    // Again, if both have the same sign (and neither one is 0),
    // no intersection is possible.
    if (d1 > 0 && d2 > 0) return false;
    if (d1 < 0 && d2 < 0) return false;

	return true;
}