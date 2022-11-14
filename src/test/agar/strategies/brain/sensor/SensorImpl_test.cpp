#include <gtest/gtest.h>

#include "agar/Bubble.h"
#include "agar/brain/SensorImpl.h"

#include <algorithm>

using namespace agarai;

class ExampleSensor: public ::testing::Test {
  public:
    ExampleSensor(): me(new Bubble(51, 51, 52)), limits(Rectangle{0, 100, 0, 100}) {
      // NE
      neighbors.push_back(new Bubble(55, 55, 55));
      neighbors.push_back(new Bubble(65, 65, 65));
      // NW
      neighbors.push_back(new Bubble(50, 45, 55));
      neighbors.push_back(new Bubble(50, 35, 65));
      // SW
      neighbors.push_back(new Bubble(45, 45, 45));
      neighbors.push_back(new Bubble(35, 35, 35));
      // SE
      neighbors.push_back(new Bubble(50, 55, 45));
      neighbors.push_back(new Bubble(50, 65, 35));
    }

    ~ExampleSensor() {
      for (auto bubble: neighbors) {
        delete bubble;
      }
      neighbors.clear();
    }

  protected:
    void SetUp() override {
      auto context = DecisionContext {
        me.get(),
        limits,
        neighbors,
      };
      sensor = std::unique_ptr<SensorImpl>(new SensorImpl(context));
    }

    void TearDown() override {
      sensor.reset();
    }

    std::unique_ptr<Bubble> me;
    Rectangle limits;
    std::vector<Bubble*> neighbors;
    std::unique_ptr<SensorImpl> sensor;
};

TEST_F(ExampleSensor, AllSensorsWorkProperly) {
  // self sensors
  EXPECT_EQ(sensor->get(SensorType::LOCATION_X), 0.51f);
  EXPECT_EQ(sensor->get(SensorType::LOCATION_Y), 0.52f);
  EXPECT_FLOAT_EQ(sensor->get(SensorType::MASS), SensorImpl::normalizeMass(me.get()));

  auto b0 = neighbors[0];
  auto b1 = neighbors[1];
  auto b2 = neighbors[2];
  auto b4 = neighbors[4];
  auto b6 = neighbors[6];

  // biggest neighbors
  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::BIGGEST_NEIGHBOR_NE_MASS),
    SensorImpl::normalizeMass(b1));
  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::BIGGEST_NEIGHBOR_NE_DISTANCE),
    SensorImpl::normalizeDistance(me.get(), b1, limits));

  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::BIGGEST_NEIGHBOR_NW_MASS),
    SensorImpl::normalizeMass(b2));
  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::BIGGEST_NEIGHBOR_NW_DISTANCE),
    SensorImpl::normalizeDistance(me.get(), b2, limits));

  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::BIGGEST_NEIGHBOR_SW_MASS),
    SensorImpl::normalizeMass(b4));
  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::BIGGEST_NEIGHBOR_SW_DISTANCE),
    SensorImpl::normalizeDistance(me.get(), b4, limits));

  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::BIGGEST_NEIGHBOR_SE_MASS),
    SensorImpl::normalizeMass(b6));
  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::BIGGEST_NEIGHBOR_SE_DISTANCE),
    SensorImpl::normalizeDistance(me.get(), b6, limits));

  // smallest neighbors
  EXPECT_FLOAT_EQ(sensor->get(SensorType::SMALLEST_NEIGHBOR_NE_MASS), 0.00055);
  EXPECT_FLOAT_EQ(sensor->get(SensorType::SMALLEST_NEIGHBOR_NE_DISTANCE), 0.035355341);

  EXPECT_FLOAT_EQ(sensor->get(SensorType::SMALLEST_NEIGHBOR_NW_MASS), 0.0005);
  EXPECT_FLOAT_EQ(sensor->get(SensorType::SMALLEST_NEIGHBOR_NW_DISTANCE), 0.047434162);

  EXPECT_FLOAT_EQ(sensor->get(SensorType::SMALLEST_NEIGHBOR_SW_MASS), 0.00035);
  EXPECT_FLOAT_EQ(sensor->get(SensorType::SMALLEST_NEIGHBOR_SW_DISTANCE), 0.16507575);

  EXPECT_FLOAT_EQ(sensor->get(SensorType::SMALLEST_NEIGHBOR_SE_MASS), 0.0005);
  EXPECT_FLOAT_EQ(sensor->get(SensorType::SMALLEST_NEIGHBOR_SE_DISTANCE), 0.057008773);

  // closest food
  EXPECT_FLOAT_EQ(sensor->get(SensorType::CLOSEST_FOOD_NE_MASS), 0);
  EXPECT_FLOAT_EQ(sensor->get(SensorType::CLOSEST_FOOD_NE_DISTANCE), 1.0);

  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::CLOSEST_FOOD_NW_MASS),
    SensorImpl::normalizeMass(b2));
  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::CLOSEST_FOOD_NW_DISTANCE),
    SensorImpl::normalizeDistance(me.get(), b2, limits));

  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::CLOSEST_FOOD_SW_MASS),
    SensorImpl::normalizeMass(b4));
  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::CLOSEST_FOOD_SW_DISTANCE),
    SensorImpl::normalizeDistance(me.get(), b4, limits));

  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::CLOSEST_FOOD_SE_MASS),
    SensorImpl::normalizeMass(b6));
  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::CLOSEST_FOOD_SE_DISTANCE),
    SensorImpl::normalizeDistance(me.get(), b6, limits));

  // biggest food
  EXPECT_FLOAT_EQ(sensor->get(SensorType::BIGGEST_FOOD_NE_MASS), 0);
  EXPECT_FLOAT_EQ(sensor->get(SensorType::BIGGEST_FOOD_NE_DISTANCE), 1);

  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::BIGGEST_FOOD_NW_MASS),
    SensorImpl::normalizeMass(b2));
  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::BIGGEST_FOOD_NW_DISTANCE),
    SensorImpl::normalizeDistance(me.get(), b2, limits));

  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::BIGGEST_FOOD_SW_MASS),
    SensorImpl::normalizeMass(b4));
  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::BIGGEST_FOOD_SW_DISTANCE),
    SensorImpl::normalizeDistance(me.get(), b4, limits));

  EXPECT_FLOAT_EQ(sensor->get(SensorType::BIGGEST_FOOD_SE_MASS),
    SensorImpl::normalizeMass(b6));
  EXPECT_FLOAT_EQ(sensor->get(SensorType::BIGGEST_FOOD_SE_DISTANCE),
    SensorImpl::normalizeDistance(me.get(), b6, limits));

  // closest threat
  EXPECT_FLOAT_EQ(
    sensor->get(SensorType::CLOSEST_THREAT_NE_MASS),
    SensorImpl::normalizeMass(b0));
  EXPECT_FLOAT_EQ(sensor->get(SensorType::CLOSEST_THREAT_NE_DISTANCE),
    SensorImpl::normalizeDistance(me.get(), b0, limits));

  EXPECT_FLOAT_EQ(sensor->get(SensorType::CLOSEST_THREAT_NW_MASS), 0);
  EXPECT_FLOAT_EQ(sensor->get(SensorType::CLOSEST_THREAT_NW_DISTANCE), 1);

  EXPECT_FLOAT_EQ(sensor->get(SensorType::CLOSEST_THREAT_SW_MASS), 0);
  EXPECT_FLOAT_EQ(sensor->get(SensorType::CLOSEST_THREAT_SW_DISTANCE),1);

  EXPECT_FLOAT_EQ(sensor->get(SensorType::CLOSEST_THREAT_SE_MASS), 0);
  EXPECT_FLOAT_EQ(sensor->get(SensorType::CLOSEST_THREAT_SE_DISTANCE), 1);
}