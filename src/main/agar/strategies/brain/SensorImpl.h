#pragma once
#include "ai/Sensor.h"
#include "agar/IBubbleControlStrategy.h"

namespace agarai {

class SensorImpl: public Sensor {
  public:
    SensorImpl(const DecisionContext context);
    ~SensorImpl();

    virtual float get(SensorType type);
  
  private:
    const DecisionContext context;
    
    bool neComputed;
    std::vector<Bubble*>  _neNeighbors;
    Bubble*               _biggestNe;

    std::vector<Bubble*> neNeighbors();
    Bubble* biggestNe();

};

}