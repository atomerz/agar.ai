#include "SensorImpl.h"
#include "agar/Bubble.h"

using namespace agarai;

SensorImpl::SensorImpl(const DecisionContext _context) : context(_context) {
}

SensorImpl::~SensorImpl() {
}

float SensorImpl::get(SensorType type) const {
  float sensorVal = 0.0;

  switch (type) {
  case SensorType::LOCATION_X:
    sensorVal = context.me->getPosition().X / context.worldLimits.width();
    break;
  case SensorType::LOCATION_Y:
    sensorVal = context.me->getPosition().Y / context.worldLimits.height();
    break;
  case SensorType::MASS: {
    const float MAX_MASS = 100000;
    sensorVal = context.me->getMass() / MAX_MASS;
    break;
  }
  default:
    assert(false);
    break;
  }

  assert(!std::isnan(sensorVal) && sensorVal >= -0.01 && sensorVal <= 1.01);

  return sensorVal;
}
