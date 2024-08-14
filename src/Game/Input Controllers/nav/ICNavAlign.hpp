#pragma once

#include "../ICWrapperInclude.hpp"

//must inherit 'Navigation::Base' for IC compatiability reasons

class ICNavAlign : public Navigation::ICNavWrapper {
public:
	ICNavAlign();

	std::unique_ptr<Navigation::NavBase> GetResult() override;
	void Draw(Camera&, Canvas&) const override;
	bool OnEvent(ftxui::Event&, Camera&) override;
	bool IsDone() const override;
	bool IsSuccessful() const override;
	void reset();
	void Update(const float& dt) override;

	void updateDescription();

	Vec2_f _from, _to;
	float _rate;

	std::string rate_as_precent = "rate%";

private:
	enum class STATE {
		UNSTARTED,
		POINTING,
		FINISHED
	};
	STATE state;
};
