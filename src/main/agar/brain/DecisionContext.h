#pragma once

#include "utils.h"

namespace agarai {

class Bubble;

struct DecisionContext {
  Bubble*					me;
  agarai::Rectangle		worldLimits;
  std::vector<Bubble*> 	visibleNeighbors;
};

}
