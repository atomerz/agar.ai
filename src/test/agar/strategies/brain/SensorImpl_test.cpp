#include <gtest/gtest.h>

#include <algorithm>

#include "agar/Bubble.h"
#include "agar/strategies/brain/SensorImpl.h"

using namespace agarai;

class ExampleSensor: public ::testing::Test {
  public:
    ExampleSensor() :me(new Bubble(50, 51, 52)) {
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
        Rectangle{0, 100, 0, 100},
        neighbors,
      };
      sensor = std::unique_ptr<SensorImpl>(new SensorImpl(context));
    }

    void TearDown() override {
      sensor.reset();
    }

    std::unique_ptr<SensorImpl> sensor;

  private:
    std::unique_ptr<Bubble> me;
    std::vector<Bubble*> neighbors;
};

TEST_F(ExampleSensor, AllSensorsWorkProperly) {
  EXPECT_EQ(sensor->get(SensorType::LOCATION_X), 0.51f);
  EXPECT_EQ(sensor->get(SensorType::LOCATION_Y), 0.52f);
  EXPECT_FLOAT_EQ(sensor->get(SensorType::MASS), 0.0005);

  EXPECT_FLOAT_EQ(sensor->get(SensorType::BIGGEST_NEIGHBOR_NE_MASS), 0.00065);
  EXPECT_FLOAT_EQ(sensor->get(SensorType::BIGGEST_NEIGHBOR_NE_DISTANCE), 0.13509256);
}