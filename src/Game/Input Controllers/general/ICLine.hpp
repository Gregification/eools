#pragma once

#include "../ICWrapperInclude.hpp"

//first time i've seen a stack overflow answer thats just "no, you cant"
//https://stackoverflow.com/questions/951234/forward-declaration-of-nested-types-classes-in-c

struct ICLine_Result {
	Vec2  from, to;
	float angle;

	//only applicable for some lines. see details of the derived.
	float precentage;
};

struct ICLine : public InputControl<ICLine_Result> {
	ICLine_Result result;

	void updateDescription();

	std::unique_ptr<ICLine_Result> GetResult() override;
	void Draw(Camera&, Canvas&) const override;
	bool OnEvent(ftxui::Event&, Camera&) override;
	bool IsDone() const override;
	bool IsSuccessful() const override;
	void reset();
	void Update(const float& dt) override;

	std::string precent_str = "rate%";

private:
	virtual Vec2 getPoint(Camera&, const Vec2_i& screenPoint) const = 0;

	enum class STATE {
		UNSTARTED,
		POINTING,
		FINISHED
	};
	STATE state;
};
