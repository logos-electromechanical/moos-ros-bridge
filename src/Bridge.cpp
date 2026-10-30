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

#include "moos-ros-bridge/Bridge.h"

#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/int64.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/float64.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int8_multi_array.hpp"
#include "std_msgs/msg/byte_multi_array.hpp"

#include <stdexcept>


template <typename T>
void MoosRosBridge::double_cb(const T &msg, const std::string moos_name)
{
    mMOOSApp.toMOOS(moos_name, msg.data);
}

template <typename T>
void MoosRosBridge::string_cb(const T &msg, const std::string moos_name)
{
    mMOOSApp.toMOOS(moos_name, msg.data);
}

template <typename T>
void MoosRosBridge::binary_cb(const T &msg, const std::string moos_name)
{
    mMOOSApp.toMOOSBinary(moos_name, msg.data);
}

void MoosRosBridge::timer_cb()
{
    if (stop) { rclcpp::shutdown(); }
}

void MoosRosBridge::init()
{
    RCLCPP_INFO(this->get_logger(), "Initializing bridge");
    for (const auto& key : mParams.toROS)
    {
        RCLCPP_INFO(this->get_logger(), "toROS key %s", key.c_str());
        if (mParams.MOOStoROS.toROS_map.at(key).rosTopicType == "std_msgs::msg::Float32")
        { 
            mPubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerPub<std_msgs::msg::Float32>(
                mParams.MOOStoROS.toROS_map.at(key).moosMsgName,
                mParams.MOOStoROS.toROS_map.at(key).rosTopicName,
                MOOSType::DOUBLE,
                ROSType::Float32,
                this->create_publisher<std_msgs::msg::Float32>(
                    mParams.MOOStoROS.toROS_map.at(key).rosTopicName, 
                    mParams.MOOStoROS.toROS_map.at(key).rosQoS
                )
            )));
        }
        else if (mParams.MOOStoROS.toROS_map.at(key).rosTopicType == "std_msgs::msg::Float64")
        {
            mPubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerPub<std_msgs::msg::Float64>(
                mParams.MOOStoROS.toROS_map.at(key).moosMsgName,
                mParams.MOOStoROS.toROS_map.at(key).rosTopicName,
                MOOSType::DOUBLE,
                ROSType::Float64,
                this->create_publisher<std_msgs::msg::Float64>(
                    mParams.MOOStoROS.toROS_map.at(key).rosTopicName, 
                    mParams.MOOStoROS.toROS_map.at(key).rosQoS
                )
            )));
        }
        else if (mParams.MOOStoROS.toROS_map.at(key).rosTopicType == "std_msgs::msg::Int32")
        {
            mPubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerPub<std_msgs::msg::Int32>(
                mParams.MOOStoROS.toROS_map.at(key).moosMsgName,
                mParams.MOOStoROS.toROS_map.at(key).rosTopicName,
                MOOSType::DOUBLE,
                ROSType::Int32,
                this->create_publisher<std_msgs::msg::Int32>(
                    mParams.MOOStoROS.toROS_map.at(key).rosTopicName, 
                    mParams.MOOStoROS.toROS_map.at(key).rosQoS
                )
            )));
        }
        else if (mParams.MOOStoROS.toROS_map.at(key).rosTopicType == "std_msgs::msg::Int64")
        {
            mPubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerPub<std_msgs::msg::Int64>(
                mParams.MOOStoROS.toROS_map.at(key).moosMsgName,
                mParams.MOOStoROS.toROS_map.at(key).rosTopicName,
                MOOSType::DOUBLE,
                ROSType::Int64,
                this->create_publisher<std_msgs::msg::Int64>(
                    mParams.MOOStoROS.toROS_map.at(key).rosTopicName, 
                    mParams.MOOStoROS.toROS_map.at(key).rosQoS
                )
            )));
        }
        else if (mParams.MOOStoROS.toROS_map.at(key).rosTopicType == "std_msgs::msg::String")
        {
            mPubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerPub<std_msgs::msg::String>(
                mParams.MOOStoROS.toROS_map.at(key).moosMsgName,
                mParams.MOOStoROS.toROS_map.at(key).rosTopicName,
                MOOSType::STRING,
                ROSType::String,
                this->create_publisher<std_msgs::msg::String>(
                    mParams.MOOStoROS.toROS_map.at(key).rosTopicName, 
                    mParams.MOOStoROS.toROS_map.at(key).rosQoS
                )
            )));
        }
        else if (mParams.MOOStoROS.toROS_map.at(key).rosTopicType == "std_msgs::msg::UInt8MultiArray")
        {
            mPubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerPub<std_msgs::msg::UInt8MultiArray>(
                mParams.MOOStoROS.toROS_map.at(key).moosMsgName,
                mParams.MOOStoROS.toROS_map.at(key).rosTopicName,
                MOOSType::BINARY,
                ROSType::UInt8Array,
                this->create_publisher<std_msgs::msg::UInt8MultiArray>(
                    mParams.MOOStoROS.toROS_map.at(key).rosTopicName, 
                    mParams.MOOStoROS.toROS_map.at(key).rosQoS
                )
            )));
        }
        else if (mParams.MOOStoROS.toROS_map.at(key).rosTopicType == "std_msgs::msg::ByteMultiArray")
        {
            mPubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerPub<std_msgs::msg::ByteMultiArray>(
                mParams.MOOStoROS.toROS_map.at(key).moosMsgName,
                mParams.MOOStoROS.toROS_map.at(key).rosTopicName,
                MOOSType::BINARY,
                ROSType::ByteArray,
                this->create_publisher<std_msgs::msg::ByteMultiArray>(
                    mParams.MOOStoROS.toROS_map.at(key).rosTopicName, 
                    mParams.MOOStoROS.toROS_map.at(key).rosQoS
                )
            )));
        } 
        else
        {
            throw std::runtime_error("Invalid type specified for MOOS to ROS");
        }
    }



    for (const auto& key : mParams.toMOOS)
    {
        RCLCPP_INFO(this->get_logger(), "toMOOS key %s", key.c_str());
        if (mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicType == "std_msgs::msg::Float32")
        {
            mSubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerSub<std_msgs::msg::Float32>(
                mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName,
                mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName,
                MOOSType::DOUBLE,
                ROSType::Float32,
                this->create_subscription<std_msgs::msg::Float32>(
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName, 
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosQoS,
                    std::function<void(const std_msgs::msg::Float32&)>(
                        std::bind(&MoosRosBridge::double_cb<std_msgs::msg::Float32>, this, std::placeholders::_1, 
                            mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName
                    ))
                )
            )));
        }
        else if (mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicType == "std_msgs::msg::Float64")
        {
            mSubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerSub<std_msgs::msg::Float64>(
                mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName,
                mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName,
                MOOSType::DOUBLE,
                ROSType::Float64,
                this->create_subscription<std_msgs::msg::Float64>(
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName, 
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosQoS,
                    std::function<void(const std_msgs::msg::Float64&)>(
                        std::bind(&MoosRosBridge::double_cb<std_msgs::msg::Float64>, this, std::placeholders::_1, 
                            mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName
                    ))
                )
            )));
        }
        else if (mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicType == "std_msgs::msg::Int32")
        {
            mSubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerSub<std_msgs::msg::Int32>(
                mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName,
                mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName,
                MOOSType::DOUBLE,
                ROSType::Int32,
                this->create_subscription<std_msgs::msg::Int32>(
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName, 
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosQoS,
                    std::function<void(const std_msgs::msg::Int32&)>(
                        std::bind(&MoosRosBridge::double_cb<std_msgs::msg::Int32>, this, std::placeholders::_1, 
                            mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName
                    ))
                )
            )));
        }
        else if (mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicType == "std_msgs::msg::Int64")
        {
            mSubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerSub<std_msgs::msg::Int64>(
                mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName,
                mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName,
                MOOSType::DOUBLE,
                ROSType::Int64,
                this->create_subscription<std_msgs::msg::Int64>(
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName, 
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosQoS,
                    std::function<void(const std_msgs::msg::Int64&)>(
                        std::bind(&MoosRosBridge::double_cb<std_msgs::msg::Int64>, this, std::placeholders::_1, 
                            mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName
                    ))
                )
            )));
        }
        else if (mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicType == "std_msgs::msg::String")
        {
            mSubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerSub<std_msgs::msg::String>(
                mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName,
                mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName,
                MOOSType::STRING,
                ROSType::String,
                this->create_subscription<std_msgs::msg::String>(
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName, 
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosQoS,
                    std::function<void(const std_msgs::msg::String&)>(
                        std::bind(&MoosRosBridge::string_cb<std_msgs::msg::String>, this, std::placeholders::_1, 
                            mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName
                    ))
                )
            )));
        }
        else if (mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicType == "std_msgs::msg::UInt8MultiArray")
        {
            mSubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerSub<std_msgs::msg::UInt8MultiArray>(
                mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName,
                mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName,
                MOOSType::BINARY,
                ROSType::UInt8Array,
                this->create_subscription<std_msgs::msg::UInt8MultiArray>(
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName, 
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosQoS,
                    std::function<void(const std_msgs::msg::UInt8MultiArray&)>(
                        std::bind(&MoosRosBridge::binary_cb<std_msgs::msg::UInt8MultiArray>, this, std::placeholders::_1, 
                            mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName
                    ))
                )
            )));
        }
        else if (mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicType == "std_msgs::msg::ByteMultiArray")
        {
            mSubVec.push_back(std::shared_ptr<MsgContainer>(new MsgContainerSub<std_msgs::msg::ByteMultiArray>(
                mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName,
                mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName,
                MOOSType::BINARY,
                ROSType::ByteArray,
                this->create_subscription<std_msgs::msg::ByteMultiArray>(
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosTopicName, 
                    mParams.ROStoMOOS.toMOOS_map.at(key).rosQoS,
                    std::function<void(const std_msgs::msg::ByteMultiArray&)>(
                        std::bind(&MoosRosBridge::binary_cb<std_msgs::msg::ByteMultiArray>, this, std::placeholders::_1, 
                            mParams.ROStoMOOS.toMOOS_map.at(key).moosMsgName
                    ))
                )
            )));
        }
        else
        {
            throw std::runtime_error("Invalid type specified for MOOS to ROS");
        }
    }
}

void MoosRosBridge::MOOSInit()
{
    mMOOSApp.AssignPublisher(mPubVec);
    mMOOSApp.Run(mParams.MOOSname.c_str(), mParams.missionFile.c_str());
}

