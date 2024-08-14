#include "ICNavAlign.hpp"

ICNavAlign::ICNavAlign()
{
	reset();
}

std::unique_ptr<Navigation::NavBase> ICNavAlign::GetResult()
{
	auto a = std::make_unique< Navigation::ALIGN>();

	//looks strange but remember the coordinate system
	a->targetRot = std::atan2f(_from.x - _to.x, -(_from.y - _to.y));

	return a;
}

void ICNavAlign::Draw(Camera&, Canvas& can) const 
{
	can.DrawPointLine(_from.x, _from.y, _to.x, _to.y, Color::Yellow);
	if(_rate != 0)
		can.DrawText(_to.x, _to.y, rate_as_precent);
}

bool ICNavAlign::OnEvent(ftxui::Event& e, Camera& c)
{
	if(e.is_mouse()){
		auto em = e.mouse();

		if (em.button != Mouse::Left) return false;
		
		switch (state) {
			case ICNavAlign::STATE::UNSTARTED: {
				_from = _to = c.mouse_screen;

				if (em.motion == Mouse::Pressed) {
					state = STATE::POINTING;
					updateDescription();
				}

			}break;

			case ICNavAlign::STATE::POINTING: {
				_to = c.mouse_screen;
				
				{
					Vec2_f diff = _to - _from;
					if (diff.x < 0)
						diff.x /= -_from.x;
					else
						diff.x /= c.formerSize.x - _from.x - 5;

					if (diff.y < 0)
						diff.y /= -_from.y;
					else
						diff.y /= c.formerSize.y - _from.y - 1;

					_rate = std::max(diff.x, diff.y);
					_rate = std::min(1.0f, _rate * 1.2f);
					rate_as_precent = std::format("{:3.1f}%", _rate * 100.0f);
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

bool ICNavAlign::IsDone() const
{
	return state == STATE::FINISHED;
}

bool ICNavAlign::IsSuccessful() const
{
	return IsDone();
}

void ICNavAlign::reset() 
{
	state = STATE::UNSTARTED;

	_from = _to = { 0 };

	_rate = 0;

	updateDescription();
}

void ICNavAlign::updateDescription()
{
	description = "Setting Alignment ";

	switch (state)
	{
	case ICNavAlign::STATE::UNSTARTED:
		description += "(1/2) click & drag to get started";
		break;
	case ICNavAlign::STATE::POINTING:
		description += "(2/2) selecting direction";
		break;
	case ICNavAlign::STATE::FINISHED:
		description += "(finished)";
		break;
	default:
		break;
	}
}

void ICNavAlign::Update(const float& dt)
{
}
