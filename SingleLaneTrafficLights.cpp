// David Strauﬂ

#include "SingleLaneTrafficLights.h"
#include <boost/thread.hpp>

SingleLaneTrafficLights::SingleLaneTrafficLights():
  numberOfCarsOnLane(0),
  eastIncomingTrafficSemaphore(1),
  westIncomingTrafficSemaphore(1),
  maximumNumberOfCarsOnLane(5),
  forceClosing(true)
{
    // decide randomly what side has green light
    sideWithGreenLight = (rand() % 2 == 0) ? 'w' : 'e';

    if (sideWithGreenLight == 'w') {
      std::cout << "West traffic light is open" << std::endl;

      eastIncomingTrafficSemaphore.wait();
    } else {
      std::cout << "East traffic light is open" << std::endl;

      westIncomingTrafficSemaphore.wait();
    }

    // if no cars wait, toggle lights every second
    switchLightsIn(1);
}

void SingleLaneTrafficLights::eastBoundPost() {
  leavingLanePost();
}

void SingleLaneTrafficLights::westBoundPost() {
  leavingLanePost();
}

void SingleLaneTrafficLights::eastBoundWait() {
  {
    // mutex allows only one car at a time to "look" at the traffic light and enter the lane 
    boost::mutex::scoped_lock lock(westTrafficLightMutex);

    // prevent cars from entering the lane from west
    westIncomingTrafficSemaphore.wait();
    numberOfCarsOnLane++;

    if (numberOfCarsOnLane < maximumNumberOfCarsOnLane) {
      std::cout << "Lane free, letting next car from west through" << std::endl;

      // there is room in the lane, allow the next one to enter
      westIncomingTrafficSemaphore.post();
    } else {
      std::cout << "Lane full, stopping next car from west " << std::endl;

      // the lane is full and was closed automatically so we dont need to force the closing
      forceClosing = false;
    }
  }
}

// check the comments from eastBoundWait
void SingleLaneTrafficLights::westBoundWait() {
  {
    boost::mutex::scoped_lock lock(eastTrafficLightMutex);

    eastIncomingTrafficSemaphore.wait();
    numberOfCarsOnLane++;

    if (numberOfCarsOnLane < maximumNumberOfCarsOnLane) {
      std::cout << "Lane free, letting next car from east through" << std::endl;

      eastIncomingTrafficSemaphore.post();
    } else {
      std::cout << "Lane full, stopping next car from east " << std::endl;
      forceClosing = false;
    }
  }
}

void SingleLaneTrafficLights::leavingLanePost() {
  {
    boost::mutex::scoped_lock lock(trafficLightSwitchMutex);
    numberOfCarsOnLane--;

    // the last car leaving the lane has to trigger a traffic light change
    if (numberOfCarsOnLane == 0) {

        if (sideWithGreenLight == 'w') {
          std::cout << "Switching lights, traffic from east can enter lane" << std::endl;

          sideWithGreenLight = 'e';

          // when the lane was not full, the last car did not close the lane entry
          // so we have to force a close
          if (forceClosing) {
            std::cout << "FORCE CLOSE WEST" << std::endl;

            westIncomingTrafficSemaphore.wait();
          } else {
            forceClosing = true;
          }

          eastIncomingTrafficSemaphore.post();
        } else {
          std::cout << "Switching lights, traffic from west can enter lane" << std::endl;

          sideWithGreenLight = 'w';

          if (forceClosing) {
            std::cout << "FORCE CLOSE EAST" << std::endl;

            eastIncomingTrafficSemaphore.wait();
          } else {
            forceClosing = true;
          }

          westIncomingTrafficSemaphore.post();
        }

    }
  }
}

// used for the automatic light switch timeout
// using a thread to not block the program
void SingleLaneTrafficLights::switchLightsIn(int seconds) {
  boost::thread lightSwitchTimerThread(boost::bind(&SingleLaneTrafficLights::lightSwitchTimer, this, seconds));
}

// the thread function to switch lights after a time interval
void SingleLaneTrafficLights::lightSwitchTimer(int seconds) {
  wait(seconds);

  // only force a switch if the lane is empty
  if (numberOfCarsOnLane == 0) {

    std::cout << "TIMEOUT SWITCH" << std::endl;

    if (sideWithGreenLight == 'w') {
      std::cout << "Switching lights, traffic from east can enter lane" << std::endl;

      sideWithGreenLight = 'e';

      westIncomingTrafficSemaphore.wait();
      eastIncomingTrafficSemaphore.post();
    } else {
      std::cout << "Switching lights, traffic from west can enter lane" << std::endl;

      sideWithGreenLight = 'w';

      eastIncomingTrafficSemaphore.wait();
      westIncomingTrafficSemaphore.post();
    }

  }

  // recreate the timer for the next switch
  switchLightsIn(seconds);
}

void SingleLaneTrafficLights::wait(int seconds) {
  clock_t endTime;
  endTime = clock() + seconds * CLOCKS_PER_SEC;

  while (clock() < endTime) {}
}