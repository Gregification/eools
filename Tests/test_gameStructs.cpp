#include <random>

#include <catch2/catch_test_macros.hpp>

#include "../Src/GameStructs.hpp"

float rndNum() {
	//arbartary but small enough float shouldn't have accuracy errors
	const int min = 1;
	const int max = 300;

	return min + (std::rand() % (max - min + 1));
}

TEST_CASE("am i stupid?") {
	using namespace gs;

	SECTION("transformations mult works") {
		Transformation_2D t(Transformation_2D::identity);

		Vec2_f p(rand());

		REQUIRE(t.applyTo(p) == p);
	}

	SECTION("transformations bijections") {
		Vec2_f p(rand());

		float z = rand();

		Transformation_2D t;
		float det;

		while ((det = t.getDeterminant_3D()) == 0 && det != 1) {
			for (auto& a : t.mat)
				for (auto& b : a)
					b = rndNum();
		}

		Vec2_f translated = t.applyTo(p);
		Vec2_f untranslated = t.getInverse().applyTo(translated);

		//the z value defaults to 1 and seems to work itself out,
		//		"if its not broke dont fix it" - mark twawawain
		REQUIRE(translated == untranslated);
	}
}
