//* The MOOS ROS Bridge
/**
 * 
 * @file
 *
 * @author Pierce Nichols <pierce@ladonrobotics.com>
 *
 * @version 2.0
 *
 * @date Oct 19 2026
 * 
 * @section LICENSE
 * 
 * Georgia Tech Research Institute (GTRI)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The MOOS ROS Bridge allows for communication between a 
 * MOOS Database and a ROS Core.  
 */
#pragma once

#include "rclcpp/rclcpp.hpp"
#include <memory>
#include <vector>
#include <thread>
#include <functional>

// MOOS Includes
#include <MOOS/libMOOS/App/MOOSApp.h>
#include <moos-ros-bridge/MOOSNode.h>
#include <moos-ros-bridge/MsgContainer.h>

// parameters
#include "moos-ros-bridge/bridge_params.hpp"

class MoosRosBridge : public rclcpp::Node
{
public:
    MoosRosBridge() : Node ("moos_ros_bridge")
    {
        mParamListener = std::make_shared<bridge_params::ParamListener>(this);
        mParams = mParamListener->get_params();
        init();
        mMOOSthread = std::make_unique<std::thread>(
            std::bind(&MoosRosBridge::MOOSInit, this));
    }

    ~MoosRosBridge() 
    {
        if (mMOOSthread) { mMOOSthread->join(); }
    }

private: 
    // methods
    void init();

    // callbacks
    template <typename T>
    void double_cb(const T &msg, const std::string moos_name);
    
    template <typename T>
    void string_cb(const T &msg, const std::string moos_name);

    template <typename T>
    void binary_cb(const T &msg, const std::string moos_name);

    // publishers & subscribers
    std::vector<shared_ptr<MsgContainer>> mPubVec;
    std::vector<shared_ptr<MsgContainer>> mSubVec;

    // parameters
    std::shared_ptr<bridge_params::ParamListener> mParamListener;
    bridge_params::Params mParams;

    // MOOS 
    MOOSNode mMOOSApp;
    void MOOSInit();
    std::unique_ptr<std::thread> mMOOSthread;

};