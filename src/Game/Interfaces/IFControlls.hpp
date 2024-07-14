#pragma once

#include <array>
#include <string>

#include "InterfaceContent.hpp"

class IFControlls : public InterfaceContent {
public:
	IFControlls();

	void Refresh() override;

private:
	static std::vector<std::vector<std::string>>
		options;
};
