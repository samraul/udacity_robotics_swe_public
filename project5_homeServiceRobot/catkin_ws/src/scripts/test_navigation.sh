#!/bin/bash
# execute commands from src
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )/.." &> /dev/null && pwd )
cd $SCRIPT_DIR

export TURTLEBOT3_MODEL="burger"
WAIT_SECS=5

# Launch turtlebot3 in a world
xterm -e " roslaunch turtlebot3_gazebo turtlebot3_world.launch " &
sleep $WAIT_SECS

# Publish robot data to TF
xterm -e " roslaunch turtlebot3_bringup turtlebot3_model.launch " &
sleep $WAIT_SECS

# Launch rviz config (will override the default one launched by bringup)
xterm -e " roslaunch turtlebot_rviz_launchers view_navigation.launch " &
sleep $WAIT_SECS

# Launch Map server with the map generated with gmapping
xterm -e " roslaunch my_map_server map_server.launch " &
sleep $WAIT_SECS

# Launch AMCL
# Note the robot is spawned at [-2.0, -0.5, 0.0] in turtlebot3_world.launch
xterm -e " roslaunch turtlebot3_navigation amcl.launch initial_pose_x:=-2.0 initial_pose_y:=-0.5" &
sleep $WAIT_SECS

# Launch MoveBase (with proper parameters)
xterm -e " roslaunch turtlebot3_navigation move_base.launch" &
sleep $WAIT_SECS

# Optionally, we can launch teleop
# xterm -e " roslaunch turtlebot3_teleop turtlebot3_teleop_key.launch " &
# sleep $WAIT_SECS

# restore folder
cd -
