#pragma once

#include "InterfaceContent.hpp"
#include "../../NetMessageType.hpp"

class IFMessager : public InterfaceContent {
public:
	IFMessager();

	OTHERMSG::ChatMsg chatMessage;
};
