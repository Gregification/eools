#pragma once

#include "GameObjectFactory.hpp"

#include "GameStructs.hpp"

std::vector<GameObjectFactory::InstanceConstructor> GameObjectFactory::ClassList{};
Class_Id GameObjectFactory::nextIdx = 0;
