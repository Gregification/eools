#pragma once

#include "InterfaceContent.hpp"

class IFOpenWindow : public InterfaceContent {
public:
	IFOpenWindow() = default;

	void OnConfirmSelection();

	void Refresh() override;
	void OnUnfocus() override;
};