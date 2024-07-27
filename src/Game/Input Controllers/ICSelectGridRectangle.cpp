#include "ICSelectGridRectangle.hpp"

ICSelectGridRectangle::ICSelectGridRectangle() : 
	state(STATE::PICK_LOWERBOUNDS)
{
	updateDescription();
}

std::optional<gs::Rectangle> ICSelectGridRectangle::GetResult() {
	return std::optional(rect);
}

void ICSelectGridRectangle::Draw( const Camera& cam, Canvas& can ) const {
	

	gs::Rectangle_i o= 
		gs::Rectangle_i{
			.pos = {0},
			.size = { can.width(), can.height()}
		}.And(rect);

	can.DrawPointLine(o.pos.x, o.pos.y, o.pos.x + o.size.x, o.pos.y, Color::Yellow);
	can.DrawPointLine(o.pos.x, o.pos.y + o.size.y, o.pos.x + o.size.x, o.pos.y + o.size.y, Color::Yellow);
	can.DrawPointLine(o.pos.x, o.pos.y + o.size.y, o.pos.x, o.pos.y, Color::Yellow);
	can.DrawPointLine(o.pos.x + o.size.x, o.pos.y, o.pos.x + o.size.x, o.pos.y + o.size.y, Color::Yellow);	
}

bool ICSelectGridRectangle::OnEvent(const ftxui::Event& e, Camera& c) {
	if (e.is_mouse()) {
		Vec2 gp = c.screenToGrid(c.mouse_screen);

		switch (state) {
		case ICSelectGridRectangle::STATE::PICK_LOWERBOUNDS:
			
			break;

		case ICSelectGridRectangle::STATE::PICK_UPPERBOUNDS:
			
			break;
		}
	}

	return false;
}

bool ICSelectGridRectangle::IsDone() const {
	return false;
}

bool ICSelectGridRectangle::IsSuccessful() const {
	return false;
}

void ICSelectGridRectangle::updateDescription() {
	description = "Selecting Rectangular Grid Area";
	switch (state) {
	case ICSelectGridRectangle::STATE::PICK_LOWERBOUNDS:
		description += "(1/2) : upper left point";
		break;

	case ICSelectGridRectangle::STATE::PICK_UPPERBOUNDS:
		description += "(2/2) : lower right point";
		break;
	}
}
