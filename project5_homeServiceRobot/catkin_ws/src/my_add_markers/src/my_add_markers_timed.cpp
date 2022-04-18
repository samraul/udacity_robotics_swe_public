#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose.h>

#include <cmath>
#include "my_pick_objects/goal_definitions.h"

const double kMarkerScale = 0.2;
ros::Publisher *marker_pub_ptr = nullptr;

/**
 * @brief Show the marker at the object location.
 *
 */
void showMarkerAtObjectLocation()
{
  // record the goal so that we check whether the robot has reached it
  const geometry_msgs::Pose& currentGoalPose = my_pick_objects::getObjectGoal().target_pose.pose;

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
 * @brief Delete the marker at the object location.
 *
 */
void deleteMarker(int32_t markerId)
{
  // delete the marker at the pick up location
  visualization_msgs::Marker marker;
  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();
  marker.ns = "basic_shapes";
  marker.id = markerId;
  marker.action = visualization_msgs::Marker::DELETE;

  ROS_INFO("Deleting marker.");
  marker_pub_ptr->publish(marker);
}

/**
 * @brief Show the marker at the drop off location.
 *
 */
void showMarkerAtDropoffLocation()
{
  const geometry_msgs::Pose& currentGoalPose = my_pick_objects::getDropOffLocationGoal().target_pose.pose;

  // create a marker at that location
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

  ROS_INFO("Adding marker at dropoff location.");
  marker_pub_ptr->publish(marker);
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "basic_shapes");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  marker_pub_ptr = &marker_pub;

  // Wait for a subscriber
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
  showMarkerAtObjectLocation();

  // wait 5 seconds and then delete it
  ros::Duration(5.0).sleep();
  deleteMarker(0);

  // wait 5 seconds and then show it at the dropoff location
  ros::Duration(5.0).sleep();
  showMarkerAtDropoffLocation();

  // done
  ros::waitForShutdown();
}
