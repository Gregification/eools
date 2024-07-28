#include "ICSelectGridRectangle.hpp"

ICSelectGridRectangle::ICSelectGridRectangle(){
	reset();
}

std::optional<gs::Rectangle> ICSelectGridRectangle::GetResult() {
	if (rect.size.x < 0) {
		rect.pos.x += rect.size.x;
		rect.size.x = std::abs(rect.size.x);
	}
	if (rect.size.y < 0) {
		rect.pos.y += rect.size.y;
		rect.size.y = std::abs(rect.size.y);
	}

	return std::optional(rect);
}

void ICSelectGridRectangle::Draw(Camera& cam, Canvas& can ) const {
	auto p = cam.gridToScreen(rect.pos);
	auto e = cam.gridToScreen(rect.pos + rect.size);
	
	//clamp to avoid canvas incorrectly clipping
	float maxx = can.width() + 1;
	float maxy = can.height() + 1;
	p.x = std::max(-1.0f, std::min(p.x, maxx));
	p.y = std::max(-1.0f, std::min(p.y, maxy));
	e.x = std::max(-1.0f, std::min(e.x, maxx));
	e.y = std::max(-1.0f, std::min(e.y, maxy));

	can.DrawPointLine(p.x  ,p.y  ,e.x  ,p.y  ,Color::Yellow);
	can.DrawPointLine(p.x  ,p.y  ,p.x  ,e.y  ,Color::Yellow);
	can.DrawPointLine(e.x  ,e.y  ,p.x  ,e.y  ,Color::Yellow);
	can.DrawPointLine(e.x  ,e.y  ,e.x  ,p.y  ,Color::Yellow);	
}

bool ICSelectGridRectangle::OnEvent(ftxui::Event& e, Camera& c) {
	if (IsDone()) return false;

	if (e.is_mouse()) {
		Vec2 gp = c.screenToGrid(c.mouse_screen);

		switch (state) {
		case ICSelectGridRectangle::STATE::PICK_LOWERBOUNDS:
			rect.pos = gp;
			break;

		case ICSelectGridRectangle::STATE::PICK_UPPERBOUNDS:
			rect.size = gp - rect.pos;
			break;

		default: break;
		}

		if (e.mouse().motion == Mouse::Released && e.mouse().button == Mouse::Left) {
			//change state
			switch (state) {
			case ICSelectGridRectangle::STATE::PICK_LOWERBOUNDS:
				state = STATE::PICK_UPPERBOUNDS;
				break;

			case ICSelectGridRectangle::STATE::PICK_UPPERBOUNDS:
				state = STATE::FINISHED;
				break;

			default: break;
			}
			updateDescription();

			return true;
		}
	}

	return false;
}

bool ICSelectGridRectangle::IsDone() const {
	return state == STATE::FINISHED;
}

bool ICSelectGridRectangle::IsSuccessful() const {
	return IsDone();
}

void ICSelectGridRectangle::reset() {
	forceFinish(false);

	state = STATE::PICK_LOWERBOUNDS;
	rect = {{0},{0}};

	updateDescription();
}

void ICSelectGridRectangle::updateDescription() {
	description = "Selecting Rectangular Grid Area : ";
	switch (state) {
	case ICSelectGridRectangle::STATE::PICK_LOWERBOUNDS:
		description += "corner (1/2)";
		break;

	case ICSelectGridRectangle::STATE::PICK_UPPERBOUNDS:
		description += "corner (2/2)";
		break;

	case ICSelectGridRectangle::STATE::FINISHED:
		description += "(finished)";
		break;
	}
}
