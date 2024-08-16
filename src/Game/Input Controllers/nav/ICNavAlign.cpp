#include "ICNavAlign.hpp"

ICNavAlign::ICNavAlign()
{
	reset();
}

std::unique_ptr<Navigation::NavBase> ICNavAlign::GetResult()
{
	auto p = icsl.GetResult();
	auto a = std::make_unique< Navigation::ALIGN>();

	if (p) {
		a->targetRot = p->angle;
		a->rate = p->precentage;
	}

	return a;
}

void ICNavAlign::Draw(Camera& c, Canvas& can) const 
{
	icsl.Draw(c, can);
}

bool ICNavAlign::OnEvent(ftxui::Event& e, Camera& c)
{
	return icsl.OnEvent(e, c);
}

bool ICNavAlign::IsDone() const
{
	return icsl.IsDone();
}

bool ICNavAlign::IsSuccessful() const
{
	return icsl.IsSuccessful();
}

void ICNavAlign::reset() 
{
	icsl.reset();
}

void ICNavAlign::updateDescription()
{
	icsl.updateDescription();
}

void ICNavAlign::Update(const float& dt)
{
	icsl.Update(dt);
}
