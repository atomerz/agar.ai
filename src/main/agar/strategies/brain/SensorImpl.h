#pragma once
#include "ai/Sensor.h"
#include "agar/IBubbleControlStrategy.h"

namespace agarai {

class SensorImpl: public Sensor {
  public:
    SensorImpl(const DecisionContext context);
    ~SensorImpl();

    virtual float get(SensorType type) const;
  
  private:
    const DecisionContext context;

};

}