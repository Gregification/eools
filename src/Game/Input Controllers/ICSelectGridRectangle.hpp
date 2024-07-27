#pragma once

#include "../InputControl.hpp"

class ICSelectGridRectangle : public InputControl<gs::Rectangle> {
public:
	ICSelectGridRectangle();

	gs::Rectangle rect;

	std::optional<gs::Rectangle> GetResult() override;
	void Draw(const Camera&, Canvas&) const override;
	bool OnEvent(const ftxui::Event&, Camera&) override;
	bool IsDone() const override;
	bool IsSuccessful() const override;

private:
	enum class STATE {
		PICK_LOWERBOUNDS,
		PICK_UPPERBOUNDS
	};
	STATE state;

	void updateDescription();
};
