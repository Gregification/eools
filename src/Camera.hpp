#pragma once

#include "GameObjects/Grid.hpp"

//TODO: some way to cache the inverse transformation

class Camera {
public:
	Camera() : mouse_screen(0), trans{Transformation_2D::identity} {

	}
	~Camera() = default;
	
	Vec2_i mouse_screen;
	Transformation_2D trans;

	const Transformation_2D& getTransformationInverse();

	inline float& offX();
	inline float& offY();

	Vec2_f getScaleVec();
	Vec2_f getOffVec();

	void Draw(Canvas& c, std::shared_ptr<Grid> g);

	template<typename T>
	Vec2 screenToGrid(Vec2_T<T> p) {
		return getTransformationInverse().applyTo(static_cast<Vec2>(p));
	}

	template<typename T>
	Vec2_f gridToScreen(Vec2_T<T> p) {
		return static_cast<Vec2_f>(trans.applyTo(p));
	}

private:
	Transformation_2D 
		c_d_trans,//former trans, used to tell if a change occured
		c_i_trans;//cached inverse
};