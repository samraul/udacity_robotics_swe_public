#include <ros/ros.h>
#include "my_pick_objects/goal_definitions.h"

// Additional libraries for ctrl+c exit support
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <signal.h>

using namespace my_pick_objects;

/**
 * @brief Function to wait until the user quits the program.
 */
int wait_for_keyboard()
{
  std::cout << "Program finished. Will wait indefinitely for ctrl+c signal before closing." << std::endl;
  ros::waitForShutdown();
  return 0;
}

int main(int argc, char **argv)
{

  const ros::Duration kWaitDuration = ros::Duration(5.0);

  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "simple_navigation_goals");

  // tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while (!ac.waitForServer(kWaitDuration))
  {
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  // -- Ready, let's go get the object

  // send goal to navigation
  ROS_INFO("On my way to pick up the object!");
  const auto &objectGoal = getObjectGoal();
  ac.sendGoal(objectGoal);

  // wait until we get a result from nab
  ac.waitForResult();

  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  {
    // we got to the desired location, pretend we are picking up the object
    ROS_INFO("Reached the object, picking it up.... (will take %u seconds)", kWaitDuration.sec);
    kWaitDuration.sleep();
  }
  else
  {
    ROS_ERROR("The base failed to reach the object location!");
    return wait_for_keyboard();
  }

  // -- Object picked up, let's go to the drop off location
  ROS_INFO("Object picked up, on my way to drop it off.");
  const auto &dropOffGoal = getDropOffLocationGoal();
  ac.sendGoal(dropOffGoal);

  // wait until we get a result from nab
  ac.waitForResult();

  // Check if the robot reached its goal
  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  {
    ROS_INFO("Reached the drop off location. Object dropped!");
  }
  else
  {
    ROS_ERROR("Failed to reach drop off location.");
    return wait_for_keyboard();
  }

  return wait_for_keyboard();
}