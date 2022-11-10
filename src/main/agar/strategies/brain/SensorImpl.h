#pragma once

#include "ai/Sensor.h"
#include "agar/IBubbleControlStrategy.h"

#include <unordered_map>

namespace agarai {

class SensorImpl: public Sensor {
  public:
    SensorImpl(const DecisionContext context);
    ~SensorImpl();

    virtual float get(SensorType type);

    enum Direction {
      NE,
      NW,
      SW,
      SE,
    };

    enum Feature {
      BIGGEST,
      SMALLEST,
      AVERAGE,
    };
  
  private:
    const DecisionContext context;

    std::unordered_map<Direction, std::vector<Bubble*> > neighborsCache;
    std::unordered_map<Feature, std::unordered_map<Direction, Bubble*> > bubblesCache;

    const std::vector<Bubble*>& neighbors(Direction direction);
    Bubble* const biggest(Direction direction);
    Bubble* const smallest(Direction direction);

};

}