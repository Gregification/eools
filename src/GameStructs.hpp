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

		Vec2_T() : Vec2_T(0, 0) {}
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

		static float Slope(const Vec2_T& a, const Vec2_T& b) {
			return (b.y - a.y) / (b.x - a.x);
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
		
		bool operator==(const Vec2_T<T>& other) const {
			return x == other.x && y == other.y;
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

		inline float getArea() const { return size.x * size.y; }
	};
	static_assert(std::is_standard_layout<gs::Rectangle>::value);

	//Affine transformations. https://en.wikipedia.org/wiki/Affine_transformation
	using Arr3 = std::array<float, 3>;
	using Mat3x3 = std::array<Arr3, 3>;
	struct Transformation_2D {
		Transformation_2D() : Transformation_2D(identity) {}
		Transformation_2D(Mat3x3 mat) : mat(mat) {}

		// | A B C |   | X |   | X |
		// | D E F | * | Y | = | Y |
		// | G H I |   | Z |   | Z |
		//  [this]     [vec]  [final]
		// - ignore final.z
		Mat3x3 mat;//[row][column] aka [y][x]. should help a bit since thats how the loops tend to go

		//no func fo effecting Z translations because would 
		//	then have to use Vec3's to do math
		inline float & scaleX() { return PT(mat, 0, 0); }
		inline float & scaleY() { return PT(mat, 1, 1); }
		inline float & scaleZ() { return PT(mat, 2, 2); }
		inline float & transX()	{ return PT(mat, 2, 0); }
		inline float & transY() { return PT(mat, 2, 1); }
		inline float & shearX() { return PT(mat, 1, 0); }
		inline float & shearY() { return PT(mat, 0, 1); } 

		void rotateBy(float rad) {
			float
				sin = std::sinf(rad),
				cos = std::cosf(rad);

			PT(mat, 0, 0) = cos;
			PT(mat, 1, 1) = cos;
			PT(mat, 1, 0) = -sin;
			PT(mat, 0, 1) = sin;
		}

		float inline getDeterminant_2D() const{
			return DETERMINANT_2x2(
				PT(mat,0,0),
				PT(mat,1,0),
				PT(mat,0,1),
				PT(mat,1,1)
			);
		}

		float inline getDeterminant_3D() const {
			return 0//skimmed this formula from wiki. seems shorter than having 3 sub determinants
				+ PT(mat,0,0) * PT(mat,1,1) * PT(mat,2,2) //+aei
				+ PT(mat,1,0) * PT(mat,2,1) * PT(mat,0,2) //+bfg
				+ PT(mat,2,0) * PT(mat,0,1) * PT(mat,1,2) //+cdh
				- PT(mat,2,0) * PT(mat,1,1) * PT(mat,0,2) //-ceg
				- PT(mat,1,0) * PT(mat,0,1) * PT(mat,2,2) //-bdi
				- PT(mat,0,0) * PT(mat,2,1) * PT(mat,1,2) //-afh
			;
 		}

		Transformation_2D getInverse() const {
			Mat3x3 inv;

			float det_3D = getDeterminant_3D();

			// ...
			if (det_3D == 0) 
				det_3D = 1;

			for (int x = 0; x < 3; x++)
				for (int y = 0; y < 3; y++) {
					//cofactor matrix
					//& reflected
					int
						x1 = (x + 1) % 3,
						x2 = (x + 2) % 3,
						y1 = (y + 1) % 3,
						y2 = (y + 2) % 3;

					PT(inv,y,x) = DETERMINANT_2x2(
						PT(mat,x1, y1),
						PT(mat,x2, y1),
						PT(mat,x1, y2),
						PT(mat,x2, y2)
					) / det_3D;
				}

			return Transformation_2D(inv);
		}

		template<typename T>
		Vec2_T<T> applyTo(Vec2_T<T> vec, float vecz = 1) const {
			//can overlook Z out since G and H might as well be 0 as far as 2d is concerned
			// | A B C |   | X |   | X |
			// | D E F | * | Y | = | Y |
			// | G H I |   | Z |   | Z |
			//  [this]     [vec]   [out]
			Vec2_T<T> out;
			out.x =	
				  PT(mat, 0, 0) * vec.x //ax
				+ PT(mat, 1, 0) * vec.y //by
				+ PT(mat, 2, 0) * vecz; //cz
			out.y = 
				  PT(mat, 0, 1) * vec.x //dx
				+ PT(mat, 1, 1) * vec.y //ey
				+ PT(mat, 2, 1) * vecz; //fz

			return out;
		}

		//shouldnt need anything larger than this
		Arr3 mul(const Arr3& arr) const {
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
		static constexpr Mat3x3 zero = { {
				{{0,0,0}},
				{{0,0,0}},
				{{0,0,0}}
			} };
	};
	static_assert(std::is_standard_layout<gs::Transformation_2D>::value);

	struct Transform {
		float rotation;//radians. i dont know enough to make it effeciently use some matrix stuff, cant figure how to read back the angle
		float angularVelocity;

		float acceleration;

		Vec2 position, velocity;

		Transform() :
			acceleration(0),
			rotation(0),
			angularVelocity(0),
			position(0, 0),
			velocity(0, 0)
		{}

		void Update(float dt) {
			rotation += dt * angularVelocity;

			velocity.x += acceleration * dt * std::cosf(rotation);
			velocity.y += acceleration * dt * std::sinf(rotation);

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