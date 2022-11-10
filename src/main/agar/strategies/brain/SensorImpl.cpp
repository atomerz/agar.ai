#include "SensorImpl.h"
#include "agar/Bubble.h"

#include <algorithm>

using namespace agarai;

SensorImpl::SensorImpl(const DecisionContext _context) : context(_context) {
}

SensorImpl::~SensorImpl() {
}

static float normalizeMass(Bubble* bubble) {
  if (bubble == nullptr) return 0;

  constexpr float MAX_MASS = 100000;
  return bubble->getMass() / MAX_MASS;
}

static float normalizeDistance(Bubble* b1, Bubble* b2, float maxDistance) {
  if (b2 == nullptr) return 1;

  return (b1->getPosition().distance(b2->getPosition())) / maxDistance;
}

float SensorImpl::get(SensorType type) {
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
    sensorVal = normalizeMass(context.me);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_NE_MASS: {
    auto bubble = biggest(Direction::NE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_NE_DISTANCE: {
    auto bubble = biggest(Direction::NE);
    sensorVal = normalizeDistance(context.me, bubble, MAX_DISTANCE);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_NW_MASS: {
    auto bubble = biggest(Direction::NW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_NW_DISTANCE: {
    auto bubble = biggest(Direction::NW);
    sensorVal = normalizeDistance(context.me, bubble, MAX_DISTANCE);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_SW_MASS: {
    auto bubble = biggest(Direction::SW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_SW_DISTANCE: {
    auto bubble = biggest(Direction::SW);
    sensorVal = normalizeDistance(context.me, bubble, MAX_DISTANCE);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_SE_MASS: {
    auto bubble = biggest(Direction::SE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_SE_DISTANCE: {
    auto bubble = biggest(Direction::SE);
    sensorVal = normalizeDistance(context.me, bubble, MAX_DISTANCE);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_NE_MASS: {
    auto bubble = smallest(Direction::NE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_NE_DISTANCE: {
    auto bubble = smallest(Direction::NE);
    sensorVal = normalizeDistance(context.me, bubble, MAX_DISTANCE);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_NW_MASS: {
    auto bubble = smallest(Direction::NW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_NW_DISTANCE: {
    auto bubble = smallest(Direction::NW);
    sensorVal = normalizeDistance(context.me, bubble, MAX_DISTANCE);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_SW_MASS: {
    auto bubble = smallest(Direction::SW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_SW_DISTANCE: {
    auto bubble = smallest(Direction::SW);
    sensorVal = normalizeDistance(context.me, bubble, MAX_DISTANCE);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_SE_MASS: {
    auto bubble = smallest(Direction::SE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_SE_DISTANCE: {
    auto bubble = smallest(Direction::SE);
    sensorVal = normalizeDistance(context.me, bubble, MAX_DISTANCE);
    break;
  }
  case SensorType::FOOD_DENSITY_NE:
  case SensorType::FOOD_DENSITY_NW:
  case SensorType::FOOD_DENSITY_SW:
  case SensorType::FOOD_DENSITY_SE:
    break;
  default:
    assert(false);
    break;
  }

  assert(!std::isnan(sensorVal) && sensorVal >= -0.01 && sensorVal <= 1.01);

  return sensorVal;
}

static bool checkDirection(SensorImpl::Direction direction, Coord2d p1, Coord2d p2) {
  switch (direction)
  {
  case SensorImpl::Direction::NE:
    return p2.X >= p1.X && p2.Y >= p1.Y;
  case SensorImpl::Direction::NW:
    return p2.X <= p1.X && p2.Y >= p1.Y;
  case SensorImpl::Direction::SW:
    return p2.X <= p1.X && p2.Y <= p1.Y;
  case SensorImpl::Direction::SE:
    return p2.X >= p1.X && p2.Y <= p1.Y;
  default:
    assert(false);
  }
}

const std::vector<Bubble*>& SensorImpl::neighbors(Direction direction) {
  auto cachedResult = neighborsCache.find(direction);
  if (cachedResult != neighborsCache.end()) {
    return cachedResult->second;
  }

  auto& neighborsInDirection = neighborsCache[direction];
  std::copy_if(context.visibleNeighbors.begin(), context.visibleNeighbors.end(),
    std::back_inserter(neighborsInDirection),
    [me = context.me, direction](auto bubble) {
      return checkDirection(direction, me->getPosition(), bubble->getPosition());
    });

  return neighborsInDirection;
}

Bubble* const SensorImpl::biggest(Direction direction) {
  auto l1Result = bubblesCache.find(Feature::BIGGEST);
  if (l1Result != bubblesCache.end()) {
    auto& l2Cache = l1Result->second;
    auto result = l2Cache.find(direction);
    if (result != l2Cache.end()) {
      return result->second;
    }
  }

  auto& l2Cache = bubblesCache[Feature::BIGGEST];

  auto& neighborsInDirection = neighbors(direction);
  auto biggest = std::max_element(neighborsInDirection.begin(), neighborsInDirection.end(), [](auto b1, auto b2){
    return b1->getMass() < b2->getMass();
  });

  if (biggest != neighborsInDirection.end()) {
    l2Cache[direction] = *biggest;
  } else {
    l2Cache[direction] = nullptr;
  }

  return l2Cache[direction];
}

Bubble* const SensorImpl::smallest(Direction direction) {
  auto l1Result = bubblesCache.find(Feature::SMALLEST);
  if (l1Result != bubblesCache.end()) {
    auto& l2Cache = l1Result->second;
    auto result = l2Cache.find(direction);
    if (result != l2Cache.end()) {
      return result->second;
    }
  }

  auto& l2Cache = bubblesCache[Feature::SMALLEST];

  auto& neighborsInDirection = neighbors(direction);
  auto smallest = std::min_element(neighborsInDirection.begin(), neighborsInDirection.end(), [](auto b1, auto b2){
    return b1->getMass() < b2->getMass();
  });

  if (smallest != neighborsInDirection.end()) {
    l2Cache[direction] = *smallest;
  } else {
    l2Cache[direction] = nullptr;
  }

  return l2Cache[direction];
}
