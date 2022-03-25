#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"
#include  <string>

// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

// DONE: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
// This function should publish the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities
bool handle_drive_request(::ball_chaser::DriveToTargetRequest& req, ::ball_chaser::DriveToTargetResponse& res)
{
    // publish the command
    geometry_msgs::Twist twist;
    twist.linear.x = req.linear_x;
    twist.angular.z = req.angular_z;
    motor_command_publisher.publish(twist);

    // generate response
    res.msg_feedback = "Setting linX:" + std::to_string(req.linear_x) + " angZ:" + std::to_string(req.angular_z);
    ROS_INFO_STREAM(res.msg_feedback);

    return true;
}

int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot");

    // Create a ROS NodeHandle object
    ros::NodeHandle n;

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // DONE: Define a drive /ball_chaser/command_robot service with a handle_drive_request callback function
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);    

    // DONE: Delete the loop, move the code to the inside of the callback function and make the necessary changes to publish the requested velocities instead of constant values

    // DONE: Handle ROS communication events
    ros::spin();

    return 0;
}