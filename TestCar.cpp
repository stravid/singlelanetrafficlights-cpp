// David Strauﬂ

#include "TestCar.h"

/*

TestCar is used to test the SingleLaneTrafficLights class. Each car can have it's own random travel time.
In reality unrealistic, this is used to have a better testcase.

*/

TestCar::TestCar(boost::mutex* coutMutex, int id, char direction, SingleLaneTrafficLights* singleLaneTrafficLights):
  coutMutex(coutMutex),
  id(id),
  direction(direction),
  singleLaneTrafficLights(singleLaneTrafficLights)
{}

void TestCar::drive() {
  if (direction == 'e') {
    {
      boost::mutex::scoped_lock lock(*coutMutex);
    
      std::cout << "TestCar #" << id << " waits at the western traffic light" << std::endl;
    }

    singleLaneTrafficLights->eastBoundWait();

    {
      boost::mutex::scoped_lock lock(*coutMutex);
    
      std::cout << "TestCar #" << id << " enters the lane at the western traffic light" << std::endl;
    }

    wait(rand() % 10 + 1);

    {
      boost::mutex::scoped_lock lock(*coutMutex);
    
      std::cout << "TestCar #" << id << " leaves the lane at the eastern traffic light" << std::endl;
    }

    singleLaneTrafficLights->eastBoundPost();
  } else if (direction == 'w') {
    {
      boost::mutex::scoped_lock lock(*coutMutex);
    
      std::cout << "TestCar #" << id << " waits at the eastern traffic light" << std::endl;
    }

    singleLaneTrafficLights->westBoundWait();

    {
      boost::mutex::scoped_lock lock(*coutMutex);
    
      std::cout << "TestCar #" << id << " enters the lane at the eastern traffic light" << std::endl;
    }

    wait(rand() % 10 + 1);

    {
      boost::mutex::scoped_lock lock(*coutMutex);
    
      std::cout << "TestCar #" << id << " leaves the lane at the western traffic light" << std::endl;
    }

    singleLaneTrafficLights->westBoundPost();
  }
}

void TestCar::wait(int seconds) {
  clock_t endTime;
  endTime = clock() + seconds * CLOCKS_PER_SEC;

  while (clock() < endTime) {}
}