// David Strauﬂ

#ifndef SINGLE_LANE_TRAFFIC_LIGHTS_H
#define SINGLE_LANE_TRAFFIC_LIGHTS_H

#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>

class SingleLaneTrafficLights {
public:
  SingleLaneTrafficLights();
  void eastBoundWait();
	void eastBoundPost();
	void westBoundWait();
	void westBoundPost();

private:
  void leavingLanePost();
  void switchLightsIn(int seconds);
  void lightSwitchTimer(int seconds);
  void wait(int seconds);

  boost::interprocess::interprocess_semaphore eastIncomingTrafficSemaphore;
  boost::interprocess::interprocess_semaphore westIncomingTrafficSemaphore;
  boost::mutex westTrafficLightMutex;
  boost::mutex eastTrafficLightMutex;
  boost::mutex trafficLightSwitchMutex;
  int numberOfCarsOnLane;
  int maximumNumberOfCarsOnLane;
  char sideWithGreenLight;
  bool forceClosing;
};

#endif