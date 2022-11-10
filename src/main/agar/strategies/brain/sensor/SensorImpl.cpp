#include "SensorImpl.h"
#include "agar/Bubble.h"

#include <algorithm>

using namespace agarai;

SensorImpl::SensorImpl(const DecisionContext _context) : context(_context) {
  neComputed = false;
  _biggestNe = nullptr;
}

SensorImpl::~SensorImpl() {
}

float SensorImpl::get(SensorType type) {
  constexpr float MAX_MASS = 100000;

  const Coord2d blc(context.worldLimits.left, context.worldLimits.bottom);
  const Coord2d trc(context.worldLimits.right, context.worldLimits.top);
  const float MAX_DISTANCE = blc.distance(trc);

  float sensorVal = 0.0;
  switch (type) {
  case SensorType::LOCATION_X:
    sensorVal = context.me->getPosition().X / context.worldLimits.width();
    break;
  case SensorType::LOCATION_Y:
    sensorVal = context.me->getPosition().Y / context.worldLimits.height();
    break;
  case SensorType::MASS: {
    sensorVal = context.me->getMass() / MAX_MASS;
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_NE_MASS:
    biggestNe();
    if (_biggestNe != nullptr) {
      sensorVal = _biggestNe->getMass() / MAX_MASS;
    }
    break;
  case SensorType::BIGGEST_NEIGHBOR_NE_DISTANCE:
    biggestNe();
    if (_biggestNe != nullptr) {
      sensorVal = (context.me->getPosition().distance(_biggestNe->getPosition())) / MAX_DISTANCE;
    } else {
      sensorVal = 1;
    }
    break;
  case SensorType::BIGGEST_NEIGHBOR_NW_MASS:
    biggestNw();
    if (_biggestNw != nullptr) {
      sensorVal = _biggestNw->getMass() / MAX_MASS;
    }
    break;
  case SensorType::BIGGEST_NEIGHBOR_NW_DISTANCE:
    biggestNw();
    if (_biggestNw != nullptr) {
      sensorVal = (context.me->getPosition().distance(_biggestNw->getPosition())) / MAX_DISTANCE;
    } else {
      sensorVal = 1;
    }
    break;
  default:
    assert(false);
    break;
  }

  assert(!std::isnan(sensorVal) && sensorVal >= -0.01 && sensorVal <= 1.01);

  return sensorVal;
}

static inline bool isNe(Coord2d p1, Coord2d p2) {
  return p2.X >= p1.X && p2.Y >= p1.Y;
}

std::vector<Bubble*> SensorImpl::neNeighbors() {
  if (!neComputed) {
    std::copy_if(context.visibleNeighbors.begin(), context.visibleNeighbors.end(), std::back_inserter(_neNeighbors),
      [me = context.me](auto bubble) {
        return isNe(me->getPosition(), bubble->getPosition());
      });
    neComputed = true;
  }

  return _neNeighbors;
}

Bubble* SensorImpl::biggestNe() {
  if (_biggestNe == nullptr) {
    neNeighbors();
    auto it = std::max_element(_neNeighbors.begin(), _neNeighbors.end(), [](auto b1, auto b2){
      return b1->getMass() < b2->getMass();
    });

    if (it == _neNeighbors.end()) return nullptr;

    _biggestNe = *it;
  }

  return _biggestNe;
}

static inline bool isNw(Coord2d p1, Coord2d p2) {
  return p2.X <= p1.X && p2.Y >= p1.Y;
}

std::vector<Bubble*> SensorImpl::nwNeighbors() {
  if (!nwComputed) {
    std::copy_if(context.visibleNeighbors.begin(), context.visibleNeighbors.end(), std::back_inserter(_nwNeighbors),
      [me = context.me](auto bubble) {
        return isNw(me->getPosition(), bubble->getPosition());
      });
    nwComputed = true;
  }

  return _nwNeighbors;
}

Bubble* SensorImpl::biggestNw() {
  if (_biggestNw == nullptr) {
    nwNeighbors();
    auto it = std::max_element(_nwNeighbors.begin(), _nwNeighbors.end(), [](auto b1, auto b2){
      return b1->getMass() < b2->getMass();
    });

    if (it == _nwNeighbors.end()) return nullptr;

    _biggestNw = *it;
  }

  return _biggestNw;
}
