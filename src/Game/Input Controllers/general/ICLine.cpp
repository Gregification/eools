#include "ICLine.hpp"

void ICLine::updateDescription()
{
	description = "Selecting Line";

	switch (state)
	{
	case ICLine::STATE::UNSTARTED:
		description += "(1/2) click & drag to get started";
		break;
	case ICLine::STATE::POINTING:
		description += "(2/2) selecting direction";
		break;
	case ICLine::STATE::FINISHED:
		description += "(finished)";
		break;
	default:
		break;
	}
}

std::unique_ptr<ICLine_Result> ICLine::GetResult()
{
	result.angle = std::atan2f(
		result.from.x - result.to.x,
		result.to.y - result.from.y
	);

	auto r = std::make_unique<ICLine_Result>();
	*r = result;
	return r;
}

void ICLine::Draw(Camera&, Canvas& can) const
{
	can.DrawPointLine(
		result.from.x,
		result.from.y,
		result.to.x,
		result.to.y,
		Color::Yellow);

	if (result.precentage != 0)
		can.DrawText(
			result.to.x,
			result.to.y,
			precent_str
		);
}

bool ICLine::OnEvent(ftxui::Event& e, Camera& c)
{
	if (e.is_mouse()) {
		auto em = e.mouse();

		switch (em.button) {
		case Mouse::Left:
		//case Mouse::Right:
			break;
		default: return false;
		}

		switch (state) {
		case ICLine::STATE::UNSTARTED: {
			result.from = result.to = getPoint(c, c.mouse_screen);

			if (em.motion == Mouse::Pressed) {
				state = STATE::POINTING;
				updateDescription();
			}

		}break;

		case ICLine::STATE::POINTING: {
			result.to = getPoint(c, c.mouse_screen);

			{
				auto diff = result.to - result.from;
				if (diff.x < 0)
					diff.x /= -result.from.x;
				else
					diff.x /= c.formerSize.x - result.from.x - 5;

				if (diff.y < 0)
					diff.y /= -result.from.y;
				else
					diff.y /= c.formerSize.y - result.from.y - 1;

				result.precentage = std::max(diff.x, diff.y);
				result.precentage = std::min(1.0f, result.precentage * 1.1f);
				precent_str = std::format("{:3.1f}%", result.precentage * 100.0f);
			}

			if (em.motion == Mouse::Released) {
				state = STATE::FINISHED;
				updateDescription();
			}

		}break;

		default:
			break;
		}
	}

	return false;
}

bool ICLine::IsDone() const
{
	return state == STATE::FINISHED;
}

bool ICLine::IsSuccessful() const
{
	return IsDone();
}

void ICLine::reset()
{
	state = STATE::UNSTARTED;

	result.from = result.to = { 0 };

	result.precentage = 0;

	updateDescription();
}

void ICLine::Update(const float& dt)
{

}
