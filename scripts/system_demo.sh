#!/bin/bash

# Launch the ROS nodes
# source /opt/ros/*/setup.bash
# echo $(cd $(dirname ${BASH_SOURCE[0]}) && cd ../../../install && pwd)/setup.bash
ros2 launch moos-ros-bridge system_demo_launch.yaml &
ROSLAUNCH_PID=$!

# Launch the MOOS nodes
pAntler $(ros2 pkg prefix moos-ros-bridge)/share/moos-ros-bridge/config/bridge.moos #>& /dev/null &
PANTLER_PID=$!

uMAC $(ros2 pkg prefix moos-ros-bridge)/share/moos-ros-bridge/config/bridge.moos

printf "Killing all processes, safely ... \n"
kill -SIGINT $ROSLAUNCH_PID
kill -SIGTERM $PANTLER_PID
printf "Done killing processes.   \n"
