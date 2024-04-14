#include "Grid.hpp"

class Camera {
public:
	Camera() : offset(0,0), scale(1) {

	}
	~Camera() = default;
	
	gs::Vec2 offset;
	float scale;

	void Draw(Canvas& c, std::shared_ptr<Grid> g);
};