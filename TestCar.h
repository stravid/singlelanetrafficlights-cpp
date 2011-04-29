// David Strauﬂ

#ifndef TEST_CAR_H
#define TEST_CAR_H

#include <time.h>
#include <boost/thread/mutex.hpp>
#include <iostream>
#include "SingleLaneTrafficLights.h"

class TestCar {
public:
  TestCar(boost::mutex* coutMutex, int id, char direction, SingleLaneTrafficLights* singleLaneTrafficLights);
  void drive();

private:
  void wait(int seconds);
  boost::mutex* coutMutex;
  int id;
  char direction;
  SingleLaneTrafficLights* singleLaneTrafficLights;
};

#endif