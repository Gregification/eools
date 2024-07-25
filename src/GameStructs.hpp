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
	template<typename T>
	struct Vec2_T;
	struct Transformation_2D;

	extern std::string getPrettyString(float);

	/* quick inverse squareroot using defined behavior
	 see reply on stack overlfow of original author https://stackoverflow.com/questions/24405129/how-to-implement-fast-inverse-sqrt-without-undefined-behavior
	*/
	extern float Q_inverseSqrt(float);

	template<typename T>
	extern bool DoVecsIntersect(const Vec2_T<T>& a1, const Vec2_T<T>& a2, const Vec2_T<T>& b1, const Vec2_T<T>& b2);

	template<typename T>
	struct Vec2_T {
		T 
			x = 0,
			y = 0;

		Vec2_T() = default;
		Vec2_T(T xy) : Vec2_T(xy, xy) {};
		Vec2_T(T xv, T yv) : x(xv), y(yv) {};
		template<typename U>
		explicit Vec2_T(const Vec2_T<U>& rhs) : x(rhs.x), y(rhs.y) {};

		~Vec2_T() = default;

		static Vec2_T UnitVec(const Vec2_T& other) {
			Vec2_T ret(other.x, other.y);
			float ivsqrt = Q_inverseSqrt(other.magnitudeSquared());
			if (ivsqrt != 0) ret *= ivsqrt;
			return ret;
		}

		static Vec2_T Rot(const Vec2_T& point, const Vec2_T& pivot, const float& radian) {
			float s = std::sinf(radian);
			float c = std::cosf(radian);
			
			return Vec2_T(
				c * (point.x - pivot.x) - s * (point.y - pivot.y) + pivot.x,
				s * (point.x - pivot.x) + c * (point.y - pivot.y) + pivot.y
			);
		}

		static float Slope(const Vec2_T& a, const Vec2_T& b) {
			return (b.y - a.y) / (b.x - a.x);
		}

		T magnitude()				const { return std::sqrtf(magnitudeSquared()); }
		T magnitudeSquared()		const { return x * x + y * y; }
		T sum()						const { return x + y; }
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
		Vec2 pos;
		Vec2_f size;

		float getArea() const { return size.x * size.y; }

		//linker explodes if this is anywhere else
		template<typename T>
		bool ContainsPoint(const Vec2_T<T>& p) const {
			auto a = p.x >= pos.x;
			auto b = p.x <= pos.x + size.x;
			auto c = p.y >= pos.y;
			auto d = p.y <= pos.y + size.y;

			return a && b && c && d;
		}
	};
	static_assert(std::is_standard_layout<gs::Rectangle>::value);

	using Arr3 = std::array<float, 3>;
	using Arr2 = std::array<float, 2>;
	using Mat3x3 = std::array<Arr3, 3>;
	using Mat2x2 = std::array<Arr2, 2>;

	//Affine transformations. https://en.wikipedia.org/wiki/Affine_transformation
	//does not handle rotation (though it can)
	struct Transformation_2D {
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
		Arr3 mul(const Arr3& arr) const;

		Mat3x3 mul(const Transformation_2D&);

		static Transformation_2D identity;
		static Transformation_2D clear;
		static Transformation_2D zero;
	};
	static_assert(std::is_standard_layout<gs::Transformation_2D>::value);

	struct RotationHandler {
		Mat2x2 mat;

		/** rotation in radians */
		float _rotation;
		bool isMatBad;

		RotationHandler() :
			mat{ {{1,0},{0,1}} }, //identity
			_rotation(0),
			isMatBad(false)
		{};

		static void SetRotationMatTo(const float& radians, Mat2x2& mat);

		template<typename T>
		static Vec2_T<T> RotateThenTranslate(const Vec2_T<T> point, const Transformation_2D& t, const RotationHandler& r) {
			return t.applyTo(r.applyTo<T>(point));
		}

		/** rotates according to current rotation */
		void rotateBy(const float& relative_radians);

		const float getRotation() const { return _rotation; }

		/** sets rotation, flags rotation-mat as bad */
		void setRotation(const float& newRotation_radians);

		/** updates the rotation matrix */
		void refreshRotMat();

		/** returns self, updates it if needed*/
		RotationHandler& getUTD();

		template<typename T>
		Vec2_T<T> applyTo(const Vec2_T<T>& point, const Vec2_T<T>& pivot = {0}) const {
			float dx = point.x - pivot.x; //easier to look at, and compiler likely handles this better
			float dy = point.y - pivot.y;

			return Vec2_T<T>{
                  PT(mat, 0, 0) * dx
                + PT(mat, 1, 0) * dy,
				  PT(mat, 0, 1) * dx
				+ PT(mat, 1, 1) * dy
			} + pivot;
		}
	};

	struct Transform {
		RotationHandler rotation;
	
		float angularVelocity;

		Vec2 position, velocity, acceleration;

		Transform() :
			acceleration(0),
			angularVelocity(0)
		{}

		void Update(float dt) {

			rotation.rotateBy(angularVelocity * dt);

			velocity += acceleration * dt;

			position += velocity * dt;
		}
		
		void applyAccele(float magnitude, float direction_rot);
	};
	static_assert(std::is_standard_layout<gs::Transform>::value);

	//linker explodes when defining this elsewhere
	template<typename T, typename U>
	bool IsPointInPoly(
		const std::vector<Vec2_T<T>>& v,
		const Vec2_T<U>& p,
		const RotationHandler& r = RotationHandler()
	) {
		if (v.size() == 0) return false;

		bool even = false;

		//line from 'a' to 'b'
		auto a = r.applyTo<T>(v[v.size() - 1]);
		for (const auto& tb : v) {
			auto b = r.applyTo<T>(tb);

			//from https://wrfranklin.org/Research/Short_Notes/pnpoly.html
			if ((a.y > p.y != b.y > p.y) &&
				(p.x < (b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x))
				even = !even;

			a = b;
		}

		return even;
	}
}