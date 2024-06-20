#pragma once

#include <cmath>
#include <limits>
#include <vector>
#include <array>
#include <cmath>

#include "NetCommon/eol_net.hpp"
#include "Game_common.hpp"


//using structs to meet standard layout requirement as needed by 
namespace gs {

	/* quick inverse squareroot using defined behavior
	 see reply on stack overlfow of original author https://stackoverflow.com/questions/24405129/how-to-implement-fast-inverse-sqrt-without-undefined-behavior
	*/
	static float Q_inverseSqrt(float number) {
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

	struct Vec2 {
		float x, y;

		Vec2() : Vec2(0,0) {}
		Vec2(float x, float y) : x(x), y(y) {}
		Vec2(const Vec2& other) : x(other.x), y(other.y) {}

		static Vec2 UnitVec(const Vec2& other) {
			Vec2 ret(other.x, other.y);
			float ivsqrt = Q_inverseSqrt(other.magnitudeSquared());
			if (ivsqrt != 0) ret *= ivsqrt;
			return ret;
		}

		static Vec2 Rot(const Vec2& point, const Vec2& pivot, const float radian) {
			float s = std::sinf(radian);
			float c = std::cosf(radian);
			Vec2 ret = point - pivot;
		
			return Vec2(ret.x * c - ret.y * s, ret.x * s + ret.y * c);
		}

		bool isBad() {
			return x == NAN || y == NAN;
		}

		//https://youtu.be/Ip3X9LOh2dk?t=150
		static float determinate(const Vec2& a, const Vec2& b) {
			return (a.x + b.x) * (a.y + b.y);
		}

		float magnitude()				const { return std::sqrtf(magnitudeSquared()); }
		float magnitudeSquared()		const { return x * x + y * y; }
		float sum()						const { return x + y; }
		float dot(const Vec2& other)	const { return ((*this) * other).sum(); }

		Vec2& operator += (const Vec2& other) { x += other.x;	y += other.y;	return *this; }
		Vec2& operator -= (const Vec2& other) { x -= other.x;	y -= other.y;	return *this; }
		Vec2& operator *= (const Vec2& other) { x *= other.x;	y *= other.y;	return *this; }
		Vec2& operator /= (const Vec2& other) { x /= other.x;	y /= other.y;	return *this; }
		Vec2& operator += (const float other) { x += other;		y += other;		return *this; }
		Vec2& operator -= (const float other) { x -= other;		y -= other;		return *this; }
		Vec2& operator *= (const float other) { x *= other;		y *= other;		return *this; }
		Vec2& operator /= (const float other) { x /= other;		y /= other;		return *this; }
		Vec2 operator + (const Vec2& other) const { return Vec2(x + other.x, y - other.y); }
		Vec2 operator - (const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
		Vec2 operator * (const Vec2& other) const { return Vec2(x * other.x, y * other.y); }
		Vec2 operator / (const Vec2& other) const { return Vec2(x / other.x, y / other.y); }
		Vec2 operator + (const float other) const { return Vec2(x + other, y + other); }
		Vec2 operator - (const float other) const { return Vec2(x - other, y - other); }
		Vec2 operator * (const float other) const { return Vec2(x * other, y * other); }
		Vec2 operator / (const float other) const { return Vec2(x / other, y / other); }
	};
	static_assert(std::is_standard_layout<gs::Vec2>::value);

	struct Rectangle {
		Rectangle() : Rectangle(Vec2(0,0), Vec2(1,1)) {}
		Rectangle(Vec2 pos, Vec2 size) : pos(pos), size(size) {}

		Vec2 pos, size;
	};
	static_assert(std::is_standard_layout<gs::Rectangle>::value);

	//Affine transformations. https://en.wikipedia.org/wiki/Affine_transformation
	using Mat3x3 = std::array<std::array<float, 3>, 3>;
	struct Transformation_2D {
		Transformation_2D() : Transformation_2D(identity) {}
		Transformation_2D(Mat3x3 mat) : mat(mat) {}

		Mat3x3 mat;//[row][column]

		float & scaleX() { return mat[0][0]; }
		float & scaleY() { return mat[1][1]; }
		float & offX()	 { return mat[0][2]; }
		float & offY()   { return mat[1][2]; }
		float & shearX() { return mat[0][1]; }
		float & shearY() { return mat[1][0]; }

		void rotateBy(float rad) {
			float
				sin = std::sinf(rad),
				cos = std::cosf(rad);
			mat[0][0] += cos;
			mat[0][1] += -sin;
			mat[1][0] += sin;
			mat[1][1] += cos;
		}

		void applyTo(Vec2& vec) const {
			// | A B C |   | X |   | X |
			// | D E F | * | Y | = | Y |
			// | G H I |   | Z |   | Z |
			//  [this]      [vec]   [final]
			// ignore Z
			const float vecz = 1;

			vec.x =	mat[0][0] * vec.x //ax
				  + mat[0][1] * vec.y //by
				  + mat[0][1] * vecz; //cz
			vec.x =	mat[1][0] * vec.x //dx
				  + mat[1][1] * vec.y //ey
				  + mat[1][1] * vecz; //fz
		}

		static constexpr Mat3x3 identity = {{
				{{1,0,0}},	//double stuffed oreo
				{{0,1,0}},
				{{0,0,1}}	//Z = 1
			}};
		static constexpr Mat3x3 clear = {{
				{{0,0,0}},
				{{0,0,0}},
				{{0,0,1}}	//for consistency
			}};
	};
	static_assert(std::is_standard_layout<gs::Transformation_2D>::value);

	struct Transform {
		float rotation;//radians
		float angularVelocity;

		Vec2 position, velocity;

		Transform() :
			rotation(0),
			angularVelocity(0),
			position(0, 0),
			velocity(0, 0)
		{}

		void PhysUpdate(float dt) {
			rotation += dt * angularVelocity;

			position += velocity * dt;
		}

		Transformation_2D getRotationTransformation() const {
			Transformation_2D ret = Transformation_2D();
			ret.rotateBy(rotation);
			return std::move(ret);
		}
	};
	static_assert(std::is_standard_layout<gs::Transform>::value);
}