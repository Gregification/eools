#include "InterfaceContent.hpp"

class IFOpenWindow : InterfaceContent {
public:
	IFOpenWindow() {

	}

	void OnConfirmSelection();

	void Refresh() override;
	void OnDelete() override;
	void OnHide() override;
};