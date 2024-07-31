#pragma once

// naming conflict with a window api macro and ftxui::Canvas::DrawText method
#ifdef DrawText
#undef DrawText
#endif

#include <cstdint>
#include <chrono>
#include <assert.h>

#define SERVER_PORT 60000
#define STD_PARTITION_SIZE 10'000'000 //100 mill => ~200 unique parts

//since it wasent working with hash maps it uses a vector that requires it starts at 0 ... spaghettie code
#define BAD_CLASS_ID -1
typedef	uint8_t Class_Id;

//should be 0. server partition starts ar BAD_ID+1
#define BAD_ID 0
typedef uint64_t Instance_Id;	//instance id

#define DEFAULT_MsgDiff_EVERYTHING 0
typedef uint8_t
	MsgDiffType; //indicates the kind of info a message contains

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#include <math.h>
#endif

//matrix stuff
#define DETERMINANT_2x2(ii, i, iii, iv) (ii * iv - i * iii)
#define PT(MAT,X,Y) MAT[Y][X]

#define ARR(N) std::array<float,N>

/******************************************************************************
* stuff
******************************************************************************/

/*miliseconds since epoch*/
time_t inline GetTime() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

//keep incase want to go back to 32 bit 
float inline GetDT(time_t dt) {
	return dt / 1000.0;
}

template <typename T>
int signum(T val) {
	return (T(0) < val) - (val < T(0));
}

template<typename T = float>
T lerp(const T& a, const T& b, const T& r) {
	return a * (1.0f - r) + (b * r);
}

constexpr long long ct_fratorial(long long n) {
	//recursion cringe
	long long r = 1;
	for (; n > 0; n--)
		r *= n;
	return r;
}

constexpr long long ct_binomial_coef(long long n, long long k) {
	assert(k >= 0 && n >= k);

	//i could be smart about this but eh.
	return ct_fratorial(n) / (ct_fratorial(k) * ct_fratorial(n - k));
}

//should be enough, am trying to keep the system as symple as possible so nothing with refrence points. just raw global coordinates
//	maybe use the BVH setup as the coordinates? could aleivate the issue in some cases
typedef double Unit;

//units : multiply the num by these ratios to get their respective values
//see https://en.wikipedia.org/wiki/Standard_gravitational_parameter

constexpr Unit KM = 1e-3;
constexpr Unit KT = 1e-3;
constexpr Unit KJ = 1e-6;
constexpr Unit LS = KM * 299'792.458;
constexpr Unit AU = KM * 14'959'787.069'1;
constexpr Unit GRAV_CONST = 6.67430e-11; //needs adjusted if KM & KT arnt the same scale
