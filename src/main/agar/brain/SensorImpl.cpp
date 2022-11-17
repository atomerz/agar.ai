#include "SensorImpl.h"

#include <algorithm>

#include "agar/Bubble.h"
#include "ai/Params.h"


using namespace agarai;

SensorImpl::SensorImpl(const DecisionContext _context) : context(_context) {
}

SensorImpl::~SensorImpl() {
}

static float normalizeAngle(Bubble* b1, Bubble* b2) {
  if (b2 == nullptr) return 0;

  return (b1->getPosition().angle(b2->getPosition()) + M_PI) / (2 * M_PI);
}

static float normalizeAge(Bubble* b) {
  if (b == nullptr) return 0;

  constexpr float MAX_AGE = 1000;

  return b->getAge() / MAX_AGE;
}

static float normalizeHunger(Bubble* b) {
  if (b == nullptr) return 0;

  return b->getSinceLastEat() / HUNGER_TOLERANCE;
}

float SensorImpl::get(SensorType type) {
  float sensorVal = 0.0;
  switch (type) {
  case SensorType::MY_X:
    sensorVal = context.me->getPosition().X / context.worldLimits.width();
    break;
  case SensorType::MY_Y:
    sensorVal = context.me->getPosition().Y / context.worldLimits.height();
    break;
  case SensorType::MY_MASS: {
    sensorVal = normalizeMass(context.me);
    break;
  }
  case SensorType::MY_AGE: {
    sensorVal = normalizeAge(context.me);
    break;
  }
  case SensorType::MY_HUNGER: {
    sensorVal = normalizeHunger(context.me);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_NE_MASS: {
    auto bubble = biggest(Direction::NE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_NE_DISTANCE: {
    auto bubble = biggest(Direction::NE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_NW_MASS: {
    auto bubble = biggest(Direction::NW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_NW_DISTANCE: {
    auto bubble = biggest(Direction::NW);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_SW_MASS: {
    auto bubble = biggest(Direction::SW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_SW_DISTANCE: {
    auto bubble = biggest(Direction::SW);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_SE_MASS: {
    auto bubble = biggest(Direction::SE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_NEIGHBOR_SE_DISTANCE: {
    auto bubble = biggest(Direction::SE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_NE_MASS: {
    auto bubble = smallest(Direction::NE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_NE_DISTANCE: {
    auto bubble = smallest(Direction::NE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_NW_MASS: {
    auto bubble = smallest(Direction::NW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_NW_DISTANCE: {
    auto bubble = smallest(Direction::NW);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_SW_MASS: {
    auto bubble = smallest(Direction::SW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_SW_DISTANCE: {
    auto bubble = smallest(Direction::SW);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_SE_MASS: {
    auto bubble = smallest(Direction::SE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::SMALLEST_NEIGHBOR_SE_DISTANCE: {
    auto bubble = smallest(Direction::SE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::CLOSEST_FOOD_NE_MASS: {
    auto bubble = closestFood(Direction::NE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::CLOSEST_FOOD_NE_DISTANCE: {
    auto bubble = closestFood(Direction::NE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::CLOSEST_FOOD_NW_MASS: {
    auto bubble = closestFood(Direction::NW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::CLOSEST_FOOD_NW_DISTANCE: {
    auto bubble = closestFood(Direction::NW);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::CLOSEST_FOOD_SW_MASS: {
    auto bubble = closestFood(Direction::SW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::CLOSEST_FOOD_SW_DISTANCE: {
    auto bubble = closestFood(Direction::SW);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::CLOSEST_FOOD_SE_MASS: {
    auto bubble = closestFood(Direction::SE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::CLOSEST_FOOD_SE_DISTANCE: {
    auto bubble = closestFood(Direction::SE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::BIGGEST_FOOD_NE_MASS: {
    auto bubble = biggestFood(Direction::NE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_FOOD_NE_DISTANCE: {
    auto bubble = biggestFood(Direction::NE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::BIGGEST_FOOD_NW_MASS: {
    auto bubble = biggestFood(Direction::NW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_FOOD_NW_DISTANCE: {
    auto bubble = closestFood(Direction::NW);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::BIGGEST_FOOD_SW_MASS: {
    auto bubble = biggestFood(Direction::SW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_FOOD_SW_DISTANCE: {
    auto bubble = biggestFood(Direction::SW);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::BIGGEST_FOOD_SE_MASS: {
    auto bubble = biggestFood(Direction::SE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_FOOD_SE_DISTANCE: {
    auto bubble = biggestFood(Direction::SE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::CLOSEST_THREAT_NE_MASS: {
    auto bubble = closestThreat(Direction::NE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::CLOSEST_THREAT_NE_DISTANCE: {
    auto bubble = closestThreat(Direction::NE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::CLOSEST_THREAT_NW_MASS: {
    auto bubble = closestThreat(Direction::NW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::CLOSEST_THREAT_NW_DISTANCE: {
    auto bubble = closestThreat(Direction::NW);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::CLOSEST_THREAT_SW_MASS: {
    auto bubble = closestThreat(Direction::SW);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::CLOSEST_THREAT_SW_DISTANCE: {
    auto bubble = closestThreat(Direction::SW);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::CLOSEST_THREAT_SE_MASS: {
    auto bubble = closestThreat(Direction::SE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::CLOSEST_THREAT_SE_DISTANCE: {
    auto bubble = closestThreat(Direction::SE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::BIGGEST_FOOD_MASS: {
    auto bubble = biggestFood(Direction::NONE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::BIGGEST_FOOD_DISTANCE: {
    auto bubble = biggestFood(Direction::NONE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::BIGGEST_FOOD_DIRECTION: {
    auto bubble = biggestFood(Direction::NONE);
    sensorVal = normalizeAngle(context.me, bubble);
    break;
  }
  case SensorType::CLOSEST_FOOD_MASS: {
    auto bubble = closestFood(Direction::NONE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::CLOSEST_FOOD_DISTANCE: {
    auto bubble = closestFood(Direction::NONE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::CLOSEST_FOOD_DIRECTION: {
    auto bubble = closestFood(Direction::NONE);
    sensorVal = normalizeAngle(context.me, bubble);
    break;
  }
  case SensorType::CLOSEST_THREAT_MASS: {
    auto bubble = closestThreat(Direction::NONE);
    sensorVal = normalizeMass(bubble);
    break;
  }
  case SensorType::CLOSEST_THREAT_DISTANCE: {
    auto bubble = closestThreat(Direction::NONE);
    sensorVal = normalizeDistance(context.me, bubble, context.worldLimits);
    break;
  }
  case SensorType::CLOSEST_THREAT_DIRECTION: {
    auto bubble = closestThreat(Direction::NONE);
    sensorVal = normalizeAngle(context.me, bubble);
    break;
  }
  default:
    assert(false);
    break;
  }

  assert(!std::isnan(sensorVal) && sensorVal >= -0.01 && sensorVal <= 1.01);

  return sensorVal;
}

float SensorImpl::normalizeMass(Bubble* const bubble) {
  if (bubble == nullptr) return 0;

  constexpr float MAX_MASS = 100000;
  return bubble->getMass() / MAX_MASS;
}

float SensorImpl::normalizeDistance(Bubble* const b1, Bubble* const b2, Rectangle limits) {
  if (b2 == nullptr) return 1;

  const Coord2d blc(limits.left, limits.bottom);
  const Coord2d trc(limits.right, limits.top);
  const float MAX_DISTANCE = blc.distance(trc);

  return (b1->getPosition().distance(b2->getPosition())) / MAX_DISTANCE;
}

static bool checkDirection(SensorImpl::Direction direction, Coord2d p1, Coord2d p2) {
  switch (direction)
  {
  case SensorImpl::Direction::NONE:
    return true;
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

Bubble* const SensorImpl::closestFood(Direction direction) {
  auto l1Result = bubblesCache.find(Feature::CLOSEST_FOOD);
  if (l1Result != bubblesCache.end()) {
    auto& l2Cache = l1Result->second;
    auto result = l2Cache.find(direction);
    if (result != l2Cache.end()) {
      return result->second;
    }
  }

  auto& l2Cache = bubblesCache[Feature::CLOSEST_FOOD];

  auto& neighborsInDirection = neighbors(direction);
  std::vector<Bubble*> foodInDirection;
  std::copy_if(neighborsInDirection.begin(), neighborsInDirection.end(), back_inserter(foodInDirection), [me = context.me](auto b){
    return me->getMass() > b->getMass();
  });
  auto closestFood = std::min_element(foodInDirection.begin(), foodInDirection.end(), [me = context.me](auto b1, auto b2){
    return b1->getPosition().distance(me->getPosition()) < b2->getPosition().distance(me->getPosition());
  });

  if (closestFood != foodInDirection.end()) {
    l2Cache[direction] = *closestFood;
  } else {
    l2Cache[direction] = nullptr;
  }

  return l2Cache[direction];
}

Bubble* const SensorImpl::biggestFood(Direction direction) {
  auto l1Result = bubblesCache.find(Feature::BIGGEST_FOOD);
  if (l1Result != bubblesCache.end()) {
    auto& l2Cache = l1Result->second;
    auto result = l2Cache.find(direction);
    if (result != l2Cache.end()) {
      return result->second;
    }
  }

  auto& l2Cache = bubblesCache[Feature::BIGGEST_FOOD];

  auto& neighborsInDirection = neighbors(direction);
  std::vector<Bubble*> foodInDirection;
  std::copy_if(neighborsInDirection.begin(), neighborsInDirection.end(), back_inserter(foodInDirection), [me = context.me](auto b){
    return me->getMass() > b->getMass();
  });
  auto biggestFood = std::max_element(foodInDirection.begin(), foodInDirection.end(), [](auto b1, auto b2){
    return b1->getMass() < b2->getMass();
  });

  if (biggestFood != foodInDirection.end()) {
    l2Cache[direction] = *biggestFood;
  } else {
    l2Cache[direction] = nullptr;
  }

  return l2Cache[direction];
}

Bubble* const SensorImpl::closestThreat(Direction direction) {
  auto l1Result = bubblesCache.find(Feature::CLOSEST_THREAT);
  if (l1Result != bubblesCache.end()) {
    auto& l2Cache = l1Result->second;
    auto result = l2Cache.find(direction);
    if (result != l2Cache.end()) {
      return result->second;
    }
  }

  auto& l2Cache = bubblesCache[Feature::CLOSEST_THREAT];

  auto& neighborsInDirection = neighbors(direction);
  std::vector<Bubble*> foodInDirection;
  std::copy_if(neighborsInDirection.begin(), neighborsInDirection.end(), back_inserter(foodInDirection), [me = context.me](auto b){
    return me->getMass() <= b->getMass();
  });
  auto closestFood = std::min_element(foodInDirection.begin(), foodInDirection.end(), [me = context.me](auto b1, auto b2){
    // for threats we have to calculate distance from their edge
    return  (b1->getPosition().distance(me->getPosition()) - b1->getRadius()) <
            (b2->getPosition().distance(me->getPosition()) - b2->getRadius());
  });

  if (closestFood != foodInDirection.end()) {
    l2Cache[direction] = *closestFood;
  } else {
    l2Cache[direction] = nullptr;
  }

  return l2Cache[direction];
}
