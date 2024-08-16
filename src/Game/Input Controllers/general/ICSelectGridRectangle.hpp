#pragma once

#include "../ICWrapperInclude.hpp"

class ICSelectGridRectangle : public InputControl<gs::Rectangle> {
public:
	ICSelectGridRectangle();

	gs::Rectangle rect;

	std::unique_ptr<gs::Rectangle> GetResult() override;
	void Draw(Camera&, Canvas&) const override;
	bool OnEvent(ftxui::Event&, Camera&) override;
	bool IsDone() const override;
	bool IsSuccessful() const override;
	void reset();

	enum class STATE : int{
		PICK_LOWERBOUNDS,
		PICK_UPPERBOUNDS,
		FINISHED
	};
	STATE state;

	void updateDescription();
};
