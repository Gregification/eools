#pragma once

#include "../ICWrapperInclude.hpp"
#include "../../Navigation.hpp"

class ICNavAlign : public InputControl<Navigation::ALIGN> {
public:
	ICNavAlign();

	std::optional<Navigation::ALIGN> GetResult() override;
	void Draw(Camera&, Canvas&) const override;
	bool OnEvent(ftxui::Event&, Camera&) override;
	bool IsDone() const override;
	bool IsSuccessful() const override;
	void reset();

	void updateDescription();

	Vec2_f _from, _to;

private:
	enum class STATE {
		UNSTARTED,
		POINTING,
		FINISHED
	};
	STATE state;
};
