#include <random>

#include <catch2/catch_test_macros.hpp>

#include "../Src/GameStructs.hpp"

using namespace gs;

float rndNum() {
	//arbartary but small enough float shouldn't have accuracy errors
	const int min = -10;
	const int max = 15;

	return min + (std::rand() % (max - min + 1));
}

void printMat3x3(const Mat3x3& mat) {
	for (int x = 0; x < 3; x++){
		for (int y = 0; y < 3; y++) {
			std::cout << PT(mat,x,y) << " ";
		}
		std::cout << std::endl;
	}
}

void printArr(ARR(3) p) {
	for (int i = 0; i < p.size(); i++)
		std::cout << "" << p[i] << " ";
	std::cout << std::endl;
}

TEST_CASE("am i stupid?") {
	SECTION("brain damage of sorts?") {
		REQUIRE(ct_fratorial(0) == 1);
		REQUIRE(ct_fratorial(6) == 720);
		REQUIRE(ct_fratorial(9) == 362880);

		REQUIRE(ct_binomial_coef(10,5) == 252);
		REQUIRE(ct_binomial_coef(9, 7) == 36);
	}

	SECTION("transformations mult works") {
		Transformation_2D t(Transformation_2D::identity);

		Vec2_f p(rand());

		REQUIRE(t.applyTo(p) == p);
	}

	SECTION("transformations bijections") {
		ARR(3) point{ 1,2,3 };

		Transformation_2D t{{{
			{{-1,-2, 2}},
			{{ 2, 1, 1}},
			{{ 3, 4, 5}}
		}}};
		float det = t.getDeterminant_3D();

		REQUIRE(23 == det);

		auto translated = t.mul(point);
		auto untranslated = t.getInverse().mul(translated);

		//has trouble printing for some reason

		/*std::cout << "start: ";
		printArr(point);
		std::cout << "trans: ";
		printArr(translated);
		std::cout << "untra: ";
		printArr(untranslated);*/

		/*std::cout << "og mat: " << std::endl;
		printMat3x3(t.mat);*/

		/*std::cout << "inverted mat: " << std::endl;
		printMat3x3(t.getInverse().mat);*/
		
		//the z value defaults to 1 and seems to work itself out,
		//		"if its not broke dont fix it" - mark twawawain
		for(int i = 0; i < point.size(); i++)
			//if they are equal (accoutn for percision loss - eyeballed)
			REQUIRE(std::abs(point[i] - untranslated[i]) < 0.000'001f);
	}
}
