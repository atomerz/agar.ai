#include "ai/Sensor.h"

#include <cassert>

using namespace agarai;

// This converts sensor numbers to mnemonic strings.
// Useful for later processing by graph-nnet.py.
std::string agarai::sensorShortName(SensorType sensor) {
    switch(sensor) {    
    case SensorType::LOCATION_X: return "X";
    case SensorType::LOCATION_Y: return "Y";
    case SensorType::MASS: return "M";

    case SensorType::BIGGEST_NEIGHBOR_NE_MASS: return "BneM";
    case SensorType::BIGGEST_NEIGHBOR_NE_DISTANCE: return "BneD";
    case SensorType::BIGGEST_NEIGHBOR_NW_MASS: return "BnwM";
    case SensorType::BIGGEST_NEIGHBOR_NW_DISTANCE: return "BnwD";
    case SensorType::BIGGEST_NEIGHBOR_SW_MASS: return "BswM";
    case SensorType::BIGGEST_NEIGHBOR_SW_DISTANCE: return "BswD";
    case SensorType::BIGGEST_NEIGHBOR_SE_MASS: return "BseM";
    case SensorType::BIGGEST_NEIGHBOR_SE_DISTANCE: return "BseD";

    case SensorType::SMALLEST_NEIGHBOR_NE_MASS: return "SneM";
    case SensorType::SMALLEST_NEIGHBOR_NE_DISTANCE: return "SneD";
    case SensorType::SMALLEST_NEIGHBOR_NW_MASS: return "SnwM";
    case SensorType::SMALLEST_NEIGHBOR_NW_DISTANCE: return "SnwD";
    case SensorType::SMALLEST_NEIGHBOR_SW_MASS: return "SswM";
    case SensorType::SMALLEST_NEIGHBOR_SW_DISTANCE: return "SswD";
    case SensorType::SMALLEST_NEIGHBOR_SE_MASS: return "SseM";
    case SensorType::SMALLEST_NEIGHBOR_SE_DISTANCE: return "SseD";

    case SensorType::CLOSEST_FOOD_NE_MASS: return "CFneM";
    case SensorType::CLOSEST_FOOD_NE_DISTANCE: return "CFneD";
    case SensorType::CLOSEST_FOOD_NW_MASS: return "CFnwM";
    case SensorType::CLOSEST_FOOD_NW_DISTANCE: return "CFnwD";
    case SensorType::CLOSEST_FOOD_SW_MASS: return "CFswM";
    case SensorType::CLOSEST_FOOD_SW_DISTANCE: return "CFswD";
    case SensorType::CLOSEST_FOOD_SE_MASS: return "CFseM";
    case SensorType::CLOSEST_FOOD_SE_DISTANCE: return "CFseD";

    case SensorType::BIGGEST_FOOD_NE_MASS: return "BFneM";
    case SensorType::BIGGEST_FOOD_NE_DISTANCE: return "BFneD";
    case SensorType::BIGGEST_FOOD_NW_MASS: return "BnwM";
    case SensorType::BIGGEST_FOOD_NW_DISTANCE: return "BFnwD";
    case SensorType::BIGGEST_FOOD_SW_MASS: return "BFswM";
    case SensorType::BIGGEST_FOOD_SW_DISTANCE: return "BFswD";
    case SensorType::BIGGEST_FOOD_SE_MASS: return "BFseM";
    case SensorType::BIGGEST_FOOD_SE_DISTANCE: return "BFseD";

    case SensorType::CLOSEST_THREAT_NE_MASS: return "CTneM";
    case SensorType::CLOSEST_THREAT_NE_DISTANCE: return "CTneD";
    case SensorType::CLOSEST_THREAT_NW_MASS: return "CTnwM";
    case SensorType::CLOSEST_THREAT_NW_DISTANCE: return "CTnwD";
    case SensorType::CLOSEST_THREAT_SW_MASS: return "CTswM";
    case SensorType::CLOSEST_THREAT_SW_DISTANCE: return "CTswD";
    case SensorType::CLOSEST_THREAT_SE_MASS: return "CTseM";
    case SensorType::CLOSEST_THREAT_SE_DISTANCE: return "CTseD";

    case SensorType::BIGGEST_FOOD_MASS: return "BFM";
    case SensorType::BIGGEST_FOOD_DISTANCE: return "BFD";
    case SensorType::BIGGEST_FOOD_DIRECTION: return "BFd";

    case SensorType::CLOSEST_FOOD_MASS: return "CFM";
    case SensorType::CLOSEST_FOOD_DISTANCE: return "CFD";
    case SensorType::CLOSEST_FOOD_DIRECTION: return "CFd";

    case SensorType::CLOSEST_THREAT_MASS: return "CTM";
    case SensorType::CLOSEST_THREAT_DISTANCE: return "CTD";
    case SensorType::CLOSEST_THREAT_DIRECTION: return "CTd";

    default: assert(false); break;
    }
}