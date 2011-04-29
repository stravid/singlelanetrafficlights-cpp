// David Strauﬂ

#include <iostream>
#include <time.h>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "SingleLaneTrafficLights.h"
#include "TestCar.h"

const int NUMBER_OF_CARS = 101;

int main() {
  srand((unsigned int)(time(NULL)));

  TestCar *testCars[NUMBER_OF_CARS];
  boost::mutex coutMutex;
  SingleLaneTrafficLights singleLaneTrafficLights;

  for (int i = 0; i < NUMBER_OF_CARS; i++) {
    char direction = (rand() % 2 == 0) ? 'w' : 'e';
    testCars[i] = new TestCar(&coutMutex, i, direction, &singleLaneTrafficLights);
    boost::thread testCarThread(&TestCar::drive, testCars[i]);
  }

  char c;
  std::cin >> c;

  return 0;
}