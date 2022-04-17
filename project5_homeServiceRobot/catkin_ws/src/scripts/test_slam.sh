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

# Launch mapping
xterm -e " roslaunch turtlebot3_slam turtlebot3_gmapping.launch " &
sleep $WAIT_SECS

# Launch rviz config (will override the default one launched by bringup)
xterm -e " roslaunch turtlebot_rviz_launchers view_navigation.launch " &
sleep $WAIT_SECS

# Launch teleop
xterm -e " roslaunch turtlebot3_teleop turtlebot3_teleop_key.launch " &
sleep $WAIT_SECS

# restore folder
cd -
