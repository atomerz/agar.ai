#pragma once

#include <string>

namespace agarai {
  
enum Action {
  SET_DIRECTION,
  NUM_ACTIONS,
};

// Converts action numbers to mnemonic strings.
// Useful for later processing by graph-nnet.py.
std::string actionShortName(Action action);

}
