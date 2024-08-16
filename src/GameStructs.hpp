#pragma once

#include <array>
#include <cmath>
#include <limits>
#include <type_traits>
#include <vector>
#include <variant>

#include "NetCommon/eol_net.hpp"
#include "Game_common.hpp"

namespace gs {
	using Mat3x3 = std::array<ARR(3), 3>;
	using Mat2x2 = std::array<ARR(2), 2>;
	
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
		T slope()					const { return y / x; }
		T dot(const Vec2_T& o)		const { return ((*this) * o).sum(); }
		T cross(const Vec2_T& o)	const { return x * o.y - y * o.x; }
		
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

		template<typename U,
			typename std::enable_if<std::is_convertible<T, U>::value, int>::type = 0>
		operator Vec2_T<U>() const {
			return Vec2_T<U>{static_cast<U>(x), static_cast<U>(y)};
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

	template<typename T>
	struct Rectangle_T {
		Vec2_T<T> pos, size;

		float getArea() const { return size.x * size.y; }

		/** 
		* @return rectangle representing the overlapping volume. 
		*	a negative size.x or size.y implies no overlap
		*/
		template<typename U>
		Rectangle_T<T> And(const Rectangle_T<U>& o) const {
			Rectangle_T<T> ret;

			std::pair<T,T> 
				mmx = std::minmax<T>(o.pos.x, pos.x),
				mmy = std::minmax<T>(o.pos.y + o.size.y, pos.y + size.y);
			ret.size.x = mmx.first - mmx.second;
			ret.size.y = mmy.first - mmy.second;
			ret.pos.x =  mmx.second;
			ret.pos.y =  mmy.second;

			return ret;
		}

		Vec2_T<T> OverlapSize(const Rectangle_T<T>& o) const {
			auto x = std::minmax(o.pos.x, pos.x);
			auto y = std::minmax(o.pos.y + o.size.y, pos.y + size.y);
			return { x.first - x.second, y.first - y.second};
		}

		template<typename U>
		bool containsPoint(const Vec2_T<U>& p) const {
			return p.x >= pos.x
				&& p.x <= pos.x + size.x
				&& p.y >= pos.y
				&& p.y <= pos.y + size.y;
		}

		/** returns true if overlaping AREA exists*/
		template<typename U>
		bool overlaps(const Rectangle_T<U>& o, const Vec2_T<T>& offset = {0}) const {
			//a N b. top, left, right, top, bottom
			U al = pos.x + offset.x;
			U ar = pos.x + offset.x + size.x;
			U at = pos.y + offset.y;
			U ab = pos.y + offset.y + size.y;
			U bl = o.pos.x;
			U br = o.pos.x + o.size.x;
			U bt = o.pos.y;
			U bb = o.pos.y + o.size.y;

			return al < br && ar > bl && at < bb && ab > bt;
		}

		/** returns true if overlap exists, complete unlayered check*/
		template<typename U>
		bool overlaps(const std::vector<Vec2_T<U>>& poly, const Vec2_T<T>& offset = { 0 }) const {
			typedef Vec2_T<T> V2;
			//cant find a algorithm that does it without extra data structs
			//	so heres the homebrew mess that does

			int constexpr numPts = 4;
			const std::array<const V2, numPts> rectPts{ {
					static_cast<V2>(pos),			//tl
					static_cast<V2>(pos + size),	//br
					{static_cast<T>(pos.x), static_cast<T>(pos.y + size.y)},	//bl
					{static_cast<T>(pos.x + size.x), static_cast<T>(pos.y)}	//tr
				} };

			for (int i = 0; i < poly.size(); i++) {
				Vec2_T<T> pa = poly[i] + offset;
				Vec2_T<T> pb = poly[(i + 1) % poly.size()] + offset;

				//if a polygons poitn is inside the rect
				if (containsPoint(pa))
					return true;

				//check for intersection of verticies
				for (int j = 0; j < numPts; j++) {
					auto ra = rectPts[j];
					auto rb = rectPts[(j + 1) % numPts];

					//interseciton of any rect verts to current poly vert.
					const auto op = GetIntersectionScalers(pa, (pb - pa), ra, (rb - ra));
					if (op) {
						const auto arr = *op;

						//if interseciton within line bounds, 0 <= x <= 1
						if (std::abs(arr[0]) <= 1 && std::abs(arr[1]) <= 1)
							return true;
					}
				}
			}

			return false;
		}

		/** returns rectangle of region marked by 2 unsorted points*/
		static Rectangle_T<T> MakeRectangle(const Vec2_T<T>& a, const Vec2_T<T>& b) {
			std::pair<T, T> x = std::minmax(a.x, b.x);
			std::pair<T, T> y = std::minmax(a.y, b.y);
			return Rectangle_T<T>{
				{x.first, y.first},
				{x.second - x.first, y.second - y.first}
			};
		}
	};

	typedef Rectangle_T<Unit> Rectangle;
	static_assert(std::is_standard_layout<Rectangle>::value);
	typedef Rectangle_T<float> Rectangle_f;
	static_assert(std::is_standard_layout<Rectangle_f>::value);
	typedef Rectangle_T<int> Rectangle_i;
	static_assert(std::is_standard_layout<Rectangle_i>::value);

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
		ARR(3) mul(const ARR(3)& arr) const;

		Mat3x3 mul(const Transformation_2D&);

		static Transformation_2D identity;
		static Transformation_2D clear;
		static Transformation_2D zero;

		bool operator==(const Transformation_2D& o) const {
			return mat[0] == o.mat[0]
				&& mat[1] == o.mat[1]
				&& mat[2] == o.mat[2];
		}
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

		/** smallest rotation to span */
		static float RotDiff(const float& from, const float& to);

		/** distance to reach `to` form from, either direciton <+,-> (think unit circle) */
		static std::pair<float, float> RotDist(const float& form, const float& to);

		/** mods to valud rotation value */
		static inline float RotScale(const float&);

		template<typename T>
		static Vec2_T<T> RotateThenTranslate(const Vec2_T<T> point, const Transformation_2D& t, const RotationHandler& r) {
			return t.applyTo(r.applyTo<T>(point));
		}

		/** rotates according to current rotation */
		void rotateBy(const float& relative_radians);

		const float& getRotation() const { return _rotation; }

		/** sets rotation, flags rotation-mat as bad */
		void setRotation(const float& newRotation_radians);

		/** updates the rotation matrix */
		void refreshRotMat();

		/** returns self, updates it if needed*/
		RotationHandler& getUTD();

		template<typename T>
		Vec2_T<T> applyTo(const Vec2_T<T>& point) const {
			return {
				  static_cast<T>(PT(mat, 0, 0) * point.x)
				+ static_cast<T>(PT(mat, 1, 0) * point.y),
				  static_cast<T>(PT(mat, 0, 1) * point.x)
				+ static_cast<T>(PT(mat, 1, 1) * point.y)
			};
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
		void applyInlineVelocity(const float& magnitude);
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

	//https://www.youtube.com/watch?v=5FkOO1Wwb8w
	/** 
	* returns the respective scalers for the direcitonal vectors.
	*	if is colinear then returns 0 for both scalers. acceptable use
	*	of 0,0 because that shouldnt be a thing that happens in the 
	*	context of this program anyways.
	* 
	* values '0 <= x <= 1' for either vector indicate a collision within 
	*	their bounds. if both vectors have a collision in bounds 
	*	then a valid interseciton exists.
	* 
	* e.g: a line with start and end points of, respectively; A,B.
	*	the first 2 arguements would then be (A, B - A, ... ).
	*	to find the point of interseciton then would be, using 
	*	the scaler result R, A + (B - A) * R
	*/
	template<typename T>
	std::optional<std::array<T, 2>> GetIntersectionScalers(
		const Vec2_T<T>& A,
		const Vec2_T<T>& AB,
		const Vec2_T<T>& C,
		const Vec2_T<T>& CD
	) {
		T ab_cd = A.cross(AB);

		//probably not ever happening with float percision loss
		//if is colinear
		if (ab_cd == 0) {

			//if its colinear then |slope(A->B)| == |slope(C->D)| == |slope(B->C)|
			float sab = std::abs(AB.slope());
			float sbc = std::abs((C - (A + AB)).slope());

			//if is colinear parallel, or close enough too 
			//	'close enough' scaling of epsilon is determined by how similar
			//	the magnitueds are (because this is a space sim!).
			//	so the relation is
			//		similar magnitudes -> larger E => easier to be colinear
			//		large diff in magnitudes -> smaller => very easy to not be colinear
			// tldr: didnt do this here but should when i ever get to pin analysis 
			
			if (std::abs(sab - sbc) < 0.0001f) {

				//if the colinear lines intersect then
				return std::optional<std::array<T, 2>>({ T(0),T(0) });
			}

			return std::nullopt;
		}

		T i_ab_cd = 1.0f / ab_cd;

		Vec2_T<T> AC = C - A;

		T ac_cd = AC.cross(CD);
		T ab_ac = AB.cross(AC);

		return std::optional<std::array<T, 2>>({ ac_cd * i_ab_cd, -ab_ac * i_ab_cd});
	}
}