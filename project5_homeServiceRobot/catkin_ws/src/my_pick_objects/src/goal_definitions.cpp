#include <ros/ros.h>
#include "my_pick_objects/goal_definitions.h"

// Additional libraries for ctrl+c exit support
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <signal.h>

move_base_msgs::MoveBaseGoal my_pick_objects::getObjectGoal()
{
    move_base_msgs::MoveBaseGoal goal;

    // set up the frame parameters
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();

    // Define a position and orientation for the robot to reach
    goal.target_pose.pose.position.x = 1.5;
    goal.target_pose.pose.position.y = 0.25;
    goal.target_pose.pose.orientation.w = 1.0;

    return goal;
}

namespace my_pick_objects
{

    move_base_msgs::MoveBaseGoal getDropOffLocationGoal()
    {
        move_base_msgs::MoveBaseGoal goal;

        // set up the frame parameters
        goal.target_pose.header.frame_id = "map";
        goal.target_pose.header.stamp = ros::Time::now();

        // Define a position and orientation for the robot to reach
        goal.target_pose.pose.position.x = -0.75;
        goal.target_pose.pose.position.y = 2.0;

        // use a quaternion so we can specify the goal in RPY coordinates
        tf2::Quaternion q;
        q.setRPY(0, 0, -M_PI * 0.5);
        goal.target_pose.pose.orientation.x = q.getX();
        goal.target_pose.pose.orientation.y = q.getY();
        goal.target_pose.pose.orientation.z = q.getZ();
        goal.target_pose.pose.orientation.w = q.getW();

        return goal;
    }

} // end namespace
