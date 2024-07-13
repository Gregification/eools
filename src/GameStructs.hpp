#pragma once

#include <array>
#include <cmath>
#include <limits>
#include <type_traits>
#include <vector>
#include <variant>

#include "NetCommon/eol_net.hpp"
#include "Game_common.hpp"

//using structs to meet standard layout requirement as needed by 
namespace gs {
	static std::string getPrettyString(float n) {
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

	template<typename T>
	struct Vec2_T {
		T x, y;

		Vec2_T() : Vec2_T(T(), T()) {}
		Vec2_T(T xy) : Vec2_T(xy, xy) {}
		Vec2_T(T x, T y) : x(x), y(y) {}
		template<typename U>
		explicit Vec2_T(const Vec2_T<U>& rhs) : x(rhs.x), y(rhs.y) {}

		static Vec2_T UnitVec(const Vec2_T& other) {
			Vec2_T ret(other.x, other.y);
			float ivsqrt = Q_inverseSqrt(other.magnitudeSquared());
			if (ivsqrt != 0) ret *= ivsqrt;
			return ret;
		}

		static Vec2_T Rot(const Vec2_T& point, const Vec2_T& pivot, const float radian) {
			float s = std::sinf(radian);
			float c = std::cosf(radian);
			Vec2_T ret = point - pivot;

			return Vec2_T(ret.x * c - ret.y * s, ret.x * s + ret.y * c);
		}

		//https://youtu.be/Ip3X9LOh2dk?t=150
		static float determinate(const Vec2_T& a, const Vec2_T& b) {
			return (a.x + b.x) * (a.y + b.y);
		}

		T magnitude()			const { return std::sqrtf(magnitudeSquared()); }
		T magnitudeSquared()		const { return x * x + y * y; }
		T sum()					const { return x + y; }
		T dot(const Vec2_T& other)	const { return ((*this) * other).sum(); }

		template<typename U,
			typename std::enable_if<std::is_convertible<U, T>::value, int>::type = 0>
		Vec2_T& operator=(const Vec2_T<U>& rhs) {
			x = static_cast<T>(rhs.x);
			y = static_cast<T>(rhs.y);
			return *this;
		}

		template<typename U> Vec2_T<T> operator+(const Vec2_T<U>& rhs) const {
			return Vec2_T<T>{x + static_cast<T>(rhs.x), y + static_cast<T>(rhs.y)};
		} 
		template<typename U> Vec2_T<T> operator-(const Vec2_T<U>& rhs) const {
			return Vec2_T<T>{x - static_cast<T>(rhs.x), y - static_cast<T>(rhs.y)};
		}
		template<typename U> Vec2_T<T> operator*(const Vec2_T<U>& rhs) const {
			return Vec2_T<T>{x * static_cast<T>(rhs.x), y * static_cast<T>(rhs.y)};
		}
		template<typename U> Vec2_T<T> operator/(const Vec2_T<U>& rhs) const {
			return Vec2_T<T>{x / static_cast<T>(rhs.x), y / static_cast<T>(rhs.y)};
		}
		template<typename U> Vec2_T<T>& operator+=(const Vec2_T<U>& rhs) {
			x += static_cast<T>(rhs.x); y += static_cast<T>(rhs.y); return *this;
		}
		template<typename U> Vec2_T<T>& operator-=(const Vec2_T<U>& rhs) {
			x -= static_cast<T>(rhs.x); y -= static_cast<T>(rhs.y); return *this;
		}
		template<typename U> Vec2_T<T>& operator*=(const Vec2_T<U>& rhs) {
			x *= static_cast<T>(rhs.x); y *= static_cast<T>(rhs.y); return *this;
		}
		template<typename U> Vec2_T<T>& operator/=(const Vec2_T<U>& rhs) {
			x /= static_cast<T>(rhs.x); y /= static_cast<T>(rhs.y); return *this;
		}
		Vec2_T& operator += (const T& other) { x += other;		y += other;		return *this; }
		Vec2_T& operator -= (const T& other) { x -= other;		y -= other;		return *this; }
		Vec2_T& operator *= (const T& other) { x *= other;		y *= other;		return *this; }
		Vec2_T& operator /= (const T& other) { x /= other;		y /= other;		return *this; }
		Vec2_T operator + (const T& other) const { return Vec2_T(x + other, y + other); }
		Vec2_T operator - (const T& other) const { return Vec2_T(x - other, y - other); }
		Vec2_T operator * (const T& other) const { return Vec2_T(x * other, y * other); }
		Vec2_T operator / (const T& other) const { return Vec2_T(x / other, y / other); }

		operator std::string() const {
			return getPrettyString(x) + ", " + getPrettyString(y);
		}
	};
	
	typedef Vec2_T<Unit> Vec2;
	static_assert(std::is_standard_layout<Vec2>::value);
	typedef Vec2_T<float> Vec2_f;
	static_assert(std::is_standard_layout<Vec2_f>::value);
	typedef Vec2_T<double> Vec2_d;
	static_assert(std::is_standard_layout<Vec2_d>::value);
	typedef Vec2_T<int> Vec2_i;
	static_assert(std::is_standard_layout<Vec2_i>::value);

	struct Rectangle {
		Rectangle() : Rectangle(Vec2_f(0,0), Vec2_f(1,1)) {}
		Rectangle(Vec2_f pos, Vec2_f size) : pos(pos), size(size) {}

		Vec2_f pos, size;

		float getArea() const { return size.x * size.y; }
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

		template<typename T>
		Vec2_T<T> applyTo(Vec2_T<T> vec) const {
			// | A B C |   | X |   | X |
			// | D E F | * | Y | = | Y |
			// | G H I |   | Z |   | Z |
			//  [this]     [vec]  [final]
			// ignore Z
			const float vecz = 1;

			vec.x =	mat[0][0] * vec.x //ax
				  + mat[0][1] * vec.y //by
				  + mat[0][1];// * vecz; //cz
			vec.x =	mat[1][0] * vec.x //dx
				  + mat[1][1] * vec.y //ey
				  + mat[1][1];// * vecz; //fz

			return vec;
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

		void Update(float dt) {
			rotation += dt * angularVelocity;

			position += velocity * dt;
		}

		Transformation_2D getRotationTransformation() const {
			Transformation_2D ret;
			ret.rotateBy(rotation);
			return ret;
		}
	};
	static_assert(std::is_standard_layout<gs::Transform>::value);
}