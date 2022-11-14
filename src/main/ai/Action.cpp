#include "Action.h"

#include <cassert>

using namespace agarai;

// Converts action numbers to mnemonic strings.
// Useful for later processing by graph-nnet.py.
std::string agarai::actionShortName(Action action) {
    switch(action) {
    case Action::SET_DIRECTION_NORTH: return "Dn";
    case Action::SET_DIRECTION_WEST: return "Dw";
    case Action::SET_DIRECTION_SOUTH: return "Ds";
    case Action::SET_DIRECTION_EAST: return "De";
    case Action::SET_DIRECTION: return "D";
    default: assert(false); break;
    }
}