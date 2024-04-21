#pragma once

#include "GameObjectFactory.hpp"

#include "GameStructs.hpp"

std::vector<GameObjectFactory::InstanceConstructor> GameObjectFactory::ClassList = std::vector<GameObjectFactory::InstanceConstructor>(4);
class_id GameObjectFactory::nextIdx = 0;

