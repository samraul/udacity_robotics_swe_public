#ifndef GOAL_DEFINITIONS_H
#define GOAL_DEFINITIONS_H

#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Point.h>
#include <tf2/LinearMath/Quaternion.h>

namespace my_pick_objects
{

    // Define a client for to send goal requests to the move_base server through a SimpleActionClient
    typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

    /**
     * @brief Get the goal data for the object to pick up.
     */
    move_base_msgs::MoveBaseGoal getObjectGoal();

    /**
     * @brief Get the goal data for the place to drop the picked up object.
     */
    move_base_msgs::MoveBaseGoal getDropOffLocationGoal();
}

#endif