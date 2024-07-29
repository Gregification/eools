#include "InputControl.hpp"

void InputControl_Base::Update(const float& dt){};

void InputControl_Base::Draw(Camera&, Canvas&) const{};

bool InputControl_Base::OnEvent(ftxui::Event&, Camera&) { return false; };

void InputControl_Base::reset() {};

bool InputControl_Base::IsDone() const { return IsSuccessful(); }

bool InputControl_Base::IsSuccessful() const { return false; };

