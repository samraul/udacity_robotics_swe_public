#!/bin/bash
# execute commands from src
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )/.." &> /dev/null && pwd )
cd $SCRIPT_DIR

WAIT_SECS=1

git clone git@github.com:ros-perception/slam_gmapping.git
cd slam_gmapping
mv .git .git_old
rm -rf .git_old
cd ..
sleep $WAIT_SECS

git clone git@github.com:turtlebot/turtlebot_interactions.git
cd turtlebot_interactions
mv .git .git_old
rm -rf .git_old
cd ..
sleep $WAIT_SECS

git clone git@github.com:ROBOTIS-GIT/turtlebot3_msgs.git
cd turtlebot3_msgs
mv .git .git_old
rm -rf .git_old
cd ..
sleep $WAIT_SECS

git clone git@github.com:ROBOTIS-GIT/turtlebot3.git
cd turtlebot3
mv .git .git_old
rm -rf .git_old
cd ..
sleep $WAIT_SECS

git clone git@github.com:ROBOTIS-GIT/turtlebot3_simulations.git
cd turtlebot3_simulations
mv .git .git_old
rm -rf .git_old
cd ..
sleep $WAIT_SECS

cd ..
catkin_make
source ./devel/setup.bash

# restore folder
cd -
