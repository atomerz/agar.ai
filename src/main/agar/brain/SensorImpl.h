#pragma once

#include <unordered_map>

#include "DecisionContext.h"
#include "ai/Sensor.h"


namespace agarai {

class SensorImpl: public Sensor {
  public:
    SensorImpl(const DecisionContext context);
    ~SensorImpl();

    virtual float get(SensorType type);

    static float normalizeMass(Bubble* const bubble);
    static float normalizeDistance(Bubble* const b1, Bubble* const b2, Rectangle limits);

    enum Direction {
      NE,
      NW,
      SW,
      SE,
    };

    enum Feature {
      BIGGEST,
      SMALLEST,
      CLOSEST_FOOD,
      BIGGEST_FOOD,
    };
  
  private:
    const DecisionContext context;

    std::unordered_map<Direction, std::vector<Bubble*> > neighborsCache;
    std::unordered_map<Feature, std::unordered_map<Direction, Bubble*> > bubblesCache;

    const std::vector<Bubble*>& neighbors(Direction direction);
    Bubble* const biggest(Direction direction);
    Bubble* const smallest(Direction direction);
    Bubble* const closestFood(Direction direction);
    Bubble* const biggestFood(Direction direction);

};

}