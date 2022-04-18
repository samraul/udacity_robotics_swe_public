#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose.h>

#include <cmath>
#include "my_pick_objects/goal_definitions.h"

enum Phase
{
  Invalid,        // no phase set at the moment
  ObjectOnFloor,  // robot should be moving to pick up the object (marker at object)
  ObjectPickedUp, // robot at pick up location, we wait a little (no marker)
  ToDropOff       // robot should be moving towards dropoff location (marker at dropoff)
};
::geometry_msgs::Pose currentGoalPose;
Phase currentPhase = Invalid;
const double kMarkerScale = 0.1;
ros::Publisher *marker_pub_ptr = nullptr;

/**
 * @brief Transition to showing the object/marker on the floor.
 *
 */
void to_phase_objectOnFloor()
{
  // record the goal so that we check whether the robot has reached it
  currentGoalPose = my_pick_objects::getObjectGoal().target_pose.pose;
  currentPhase = ObjectOnFloor;

  // create a marker at that location
  visualization_msgs::Marker marker;
  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();
  marker.ns = "basic_shapes";
  marker.id = 0;
  marker.type = visualization_msgs::Marker::CUBE;
  marker.action = visualization_msgs::Marker::ADD;
  marker.pose.position = currentGoalPose.position;
  marker.pose.orientation = currentGoalPose.orientation;
  marker.scale.x = kMarkerScale;
  marker.scale.y = kMarkerScale;
  marker.scale.z = kMarkerScale;
  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;
  marker.lifetime = ros::Duration();

  ROS_INFO("Adding marker at object location.");
  marker_pub_ptr->publish(marker);
}

/**
 * @brief Transition to picking up the object
 *
 */
void to_phase_objectPickedUp()
{
  // transition
  currentPhase = ObjectPickedUp;

  // delete the marker at the pick up location
  visualization_msgs::Marker marker;
  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();
  marker.ns = "basic_shapes";
  marker.id = 0;
  marker.action = visualization_msgs::Marker::DELETE;

  ROS_INFO("Object reached, deleting marker.");
  marker_pub_ptr->publish(marker);
}

/**
 * @brief Transition to dropping off the object.
 *
 */
void to_phase_toDropOff()
{
  // record the goal so that we check whether the robot has reached it
  currentGoalPose = my_pick_objects::getDropOffLocationGoal().target_pose.pose;
  currentPhase = ToDropOff;
}

/**
 * @brief Transition to ending because we dropped off the object.
 * 
 */
void to_end()
{
  currentPhase = Invalid;

  // create a marker at that location because the object has been delivered
  visualization_msgs::Marker marker;
  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();
  marker.ns = "basic_shapes";
  marker.id = 1;
  marker.type = visualization_msgs::Marker::CUBE;
  marker.action = visualization_msgs::Marker::ADD;
  marker.pose.position = currentGoalPose.position;
  marker.pose.orientation = currentGoalPose.orientation;
  marker.scale.x = kMarkerScale;
  marker.scale.y = kMarkerScale;
  marker.scale.z = kMarkerScale;
  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;
  marker.lifetime = ros::Duration();

  ROS_INFO("Object dropped off, adding marker.");
  marker_pub_ptr->publish(marker);
}

void process_odom_callback(const nav_msgs::Odometry::ConstPtr &msg)
{
  if (currentPhase == ObjectOnFloor || currentPhase == ToDropOff)
  {
    // check distance against the current goal
    const double kEpsilon = 0.1;
    const auto &currentPos = msg->pose.pose.position;
    const auto &goalPos = currentGoalPose.position;
    const double dist = sqrt(pow((currentPos.x - goalPos.x), 2) + pow((currentPos.y - goalPos.y), 2) + pow((currentPos.z - goalPos.z), 2));
    // ROS_INFO("Dist %g vs epsilon %g", dist, kEpsilon);
    if (dist < kEpsilon)
    {
      if (currentPhase == ObjectOnFloor)
      {
        // picked up the object!
        to_phase_objectPickedUp();
      }
      else
      {
        // dropped off the object at the destination
        to_end();
      }
    }
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "basic_shapes");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber odomSub = n.subscribe("/odom", 1, process_odom_callback);

  marker_pub_ptr = &marker_pub;

  // Publish the marker
  while (marker_pub.getNumSubscribers() < 1)
  {
    if (!ros::ok())
    {
      return 0;
    }
    ROS_WARN_ONCE("Please create a subscriber to the marker");
    sleep(1);
  }

  ROS_INFO("Ready!");
  to_phase_objectOnFloor();

  while (ros::ok())
  {
    ros::spinOnce();

    if (currentPhase == ObjectPickedUp)
    {
      ROS_INFO("Waiting for the robot to pick up the object... (sleep)");
      ros::Duration(5.0).sleep();

      to_phase_toDropOff();
    }
    else if (currentPhase == Invalid) {
      ROS_INFO("Done! Waiting for shutdown..");
      break;
    }

    r.sleep();
  }

  ros::waitForShutdown();
}
