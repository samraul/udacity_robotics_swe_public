#include "ros/ros.h"
// #include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include  <string>


ros::ServiceClient client;
ros::Publisher motor_command_publisher;

void issue_command(const double linearSpeed, const double angularSpeed) {
    ball_chaser::DriveToTarget cmd;
    cmd.request.linear_x = linearSpeed;
    cmd.request.angular_z = angularSpeed;

    if (!client.call(cmd)) {
        ROS_ERROR("Failed to call service motor command");
    }    
}

void process_image_callback(const sensor_msgs::Image& img) {
    // bool uniform_image = true;

    // support rgb8 encoding only
    if (img.encoding != "rgb8") {
        ROS_WARN_STREAM("Unsupported image enconding: " << img.encoding);
        return;
    }

    const int leftColumnEnd = img.width / 3;
    const int centerColumnEnd = 2 * img.width / 3;

    // iterate the entire image until the flags for the zone can be set
    bool isLeft = false;
    bool isCenter = false;
    bool isRight = false;
    for(int px=0; px<img.height * img.width; ++px) {
        const int pxIdx = px * 3; // 3 channels per pixel        
        const bool isWhite = (img.data[pxIdx] == 255) && (img.data[pxIdx+1] == 255) && (img.data[pxIdx+2] == 255);
        if (isWhite) {
            const int column = px % img.width;
            isLeft = isLeft || column <= leftColumnEnd;
            isCenter = isCenter || column < centerColumnEnd;
            isRight = isRight || column >= centerColumnEnd;
            
            // early out if we have detected the ball in all zones already. This means that we are close to the ball!
            if (isLeft && isCenter && isRight) {
                break;
            }                    
        }
    }

    // Debug info:
    // std::stringstream ss;
    // ss << "Left:" << isLeft << " Center: " << isCenter << " Right:" << isRight;
    // ROS_INFO_STREAM(ss.str());

    // Table of flags vs action we want to take.
    //  idx | isLeft | isCenter | isRight | Action
    //   0      0         0         0      Can't see the ball, rotate in any direction to try to find it
    //   1      0         0         1      Rotate right
    //   2      0         1         0      Move forward
    //   3      0         1         1      Move forward while rotating right
    //   4      1         0         0      Rotate left
    //   5      1         0         1      Assert this is an error!
    //   6      1         1         0      Move forward while rotating left
    //   7      1         1         1      Stop, we are close!
    const double kDefaultSpeed = 0.5;    
    const int actionIdx = 0 | (isLeft ? 0x4 : 0) | (isCenter ? 0x2 : 0) | (isRight ? 0x1 : 0);
    switch (actionIdx) {
        case 0: // rotate right
            issue_command(0, -kDefaultSpeed);
            break;
        case 1: // rotate right
            issue_command(0, -kDefaultSpeed);
            break;
        case 2: // move forward
            issue_command(kDefaultSpeed, 0);
            break;
        case 3: // move while rotating right
            issue_command(kDefaultSpeed, -kDefaultSpeed);            
            break;
        case 4: // rotate left
            issue_command(0, kDefaultSpeed);
            break;
        case 6: // move while rotating left
            issue_command(kDefaultSpeed, kDefaultSpeed);
            break;
        case 7: // stop
            issue_command(0, 0);
            break;
        case 5:
        default:
            ROS_ERROR_STREAM("Unexpected action index: " << actionIdx);
            break;
    }
}

int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "process_image");

    // Create a ROS NodeHandle object
    ros::NodeHandle n;

    client = n.serviceClient<::ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
    ros::Subscriber cameraSub = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    ros::spin();

    return 0;
}
