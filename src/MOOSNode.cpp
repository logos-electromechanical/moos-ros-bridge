//* The MOOS ROS Bridge
/**
 *
 * @file
 *
 * @author Kevin DeMarco <kevin.demarco@gtri.gatech.edu>
 *
 * @version 1.0
 *
 * @date May 25th, 2012
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

#include <moos-ros-bridge/MOOSNode.h>
#include <moos-ros-bridge/MsgContainer.h>

#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/int64.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/float64.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int8_multi_array.hpp"
#include "std_msgs/msg/byte_multi_array.hpp"
#include <vector>
#include <iostream>

//default constructor
MOOSNode::MOOSNode()
{
	stop = false;
}

void MOOSNode::AssignPublisher(vector<shared_ptr<MsgContainer>> &msgVec)
{
	_msgVec = msgVec;
}

//default (virtual) destructor
MOOSNode::~MOOSNode()
{}

bool MOOSNode::toMOOS(std::string moosName, double data)
{
	m_Comms.Notify(moosName, data, MOOSTime());
	return true;
}

bool MOOSNode::toMOOS(std::string moosName, std::string myString)
{
	m_Comms.Notify(moosName, myString, MOOSTime());
	return true;
}

//Support for binary-string from ROS to MOOS

bool MOOSNode::toMOOSBinary(const string moosName, const vector<uint8_t> &bin)
{
    m_Comms.Notify(moosName, (void *)bin.data(), bin.size(), MOOSTime());
	return true;
}

bool MOOSNode::OnNewMail (MOOSMSG_LIST &NewMail){
	MOOSMSG_LIST::iterator p;
	for( p = NewMail.begin() ; p != NewMail.end() ; p++ ) 
	{

		CMOOSMsg & rMsg = *p;

		RCLCPP_INFO(_node->get_logger(), "Received message named %s\n", rMsg.GetKey().c_str());

		//Every time a new type is added, a new IF statement is
		//required here to handle the conversion

		vector<shared_ptr<MsgContainer>>::iterator it;

		for ( it = _msgVec.begin() ; it < _msgVec.end() ; it++ )
		{
			if((it->get()->dir() == BridgeDirection::toROS) && MOOSStrCmp(rMsg.GetKey(), it->get()->moosName()))
			{
				RCLCPP_INFO(_node->get_logger(), "Message %s matched\n", rMsg.GetKey().c_str());
				if(rMsg.IsDouble() && (it->get()->moosType() == MOOSType::DOUBLE))
				{
					switch (it->get()->rosType())
					{
						case (ROSType::Float32):
						{
							auto msg = std_msgs::msg::Float32();
							msg.data = static_cast<float>(rMsg.GetDouble());
							static_cast<MsgContainerPub<std_msgs::msg::Float32>*>(it->get())->pub->publish(msg);
							break;
						}
						case (ROSType::Float64):
						{
							auto msg = std_msgs::msg::Float64();
							msg.data = rMsg.GetDouble();
							static_cast<MsgContainerPub<std_msgs::msg::Float64>*>(it->get())->pub->publish(msg);
							break;
						}
						case (ROSType::Int32):
						{
							auto msg = std_msgs::msg::Int32();
							msg.data = static_cast<int32_t>(rMsg.GetDouble());
							static_cast<MsgContainerPub<std_msgs::msg::Int32>*>(it->get())->pub->publish(msg);
							break;
						}
						case (ROSType::Int64):
						{
							auto msg = std_msgs::msg::Int64();
							msg.data = static_cast<int64_t>(rMsg.GetDouble());
							static_cast<MsgContainerPub<std_msgs::msg::Int64>*>(it->get())->pub->publish(msg);
							break;
						}
						case (ROSType::String):
						{
							auto msg = std_msgs::msg::String();
							msg.data = std::to_string(rMsg.GetDouble());
							static_cast<MsgContainerPub<std_msgs::msg::String>*>(it->get())->pub->publish(msg);
							break;
						}
						default:
						{}
					}
				}

				else if(rMsg.IsString() && (it->get()->moosType() == MOOSType::STRING) && (it->get()->rosType() == ROSType::String))
				{
					auto msg = std_msgs::msg::String();
					msg.data = rMsg.GetString();
					static_cast<MsgContainerPub<std_msgs::msg::String>*>(it->get())->pub->publish(msg);
				}

				else if(rMsg.IsDataType(MOOS_BINARY_STRING) && (it->get()->moosType() == MOOSType::BINARY) && (it->get()->rosType() == ROSType::UInt8Array))
				{
					// Not sure this works correctly with true binary arrays -- testing required
					auto msg = std_msgs::msg::UInt8MultiArray();
					memcpy(msg.data.data(), rMsg.GetString().c_str(), rMsg.GetString().size());
					static_cast<MsgContainerPub<std_msgs::msg::UInt8MultiArray>*>(it->get())->pub->publish(msg);
				}

				else if(rMsg.IsDataType(MOOS_BINARY_STRING) && (it->get()->moosType() == MOOSType::BINARY) && (it->get()->rosType() == ROSType::ByteArray))
				{
					// Not sure this works correctly with true binary arrays -- testing required
					auto msg = std_msgs::msg::ByteMultiArray();
					memcpy(msg.data.data(), rMsg.GetString().c_str(), rMsg.GetString().size());
					static_cast<MsgContainerPub<std_msgs::msg::ByteMultiArray>*>(it->get())->pub->publish(msg);
				}

	       }//end if MOOSStrCmp
	  	}//end Iterator for
	}//end Mailbox for
	return true;
}

/*
   called by the base class when the application has made contact with
   the MOOSDB and a channel has been opened . Place code to specify what
   notifications you want to receive here .
*/
bool MOOSNode::OnConnectToServer()
{
	DoRegistrations();
	return true;
}

/*
  Called by the base class periodically. This is where you place code
  which does the work of the application
*/
bool MOOSNode::Iterate() { 
	return not stop; 
}

/*
   called by the base class before the first :: Iterate is called . Place
   startup code here − especially code which reads configuration data from the
   mission file
*/
bool MOOSNode::OnStartUp()
{
	appTick = 1;
	commsTick = 1;

	if(!m_MissionReader.GetConfigurationParam("AppTick",appTick)){
	MOOSTrace("Warning, AppTick not set.\n");
	}

	if(!m_MissionReader.GetConfigurationParam("CommsTick",commsTick)){
	MOOSTrace("Warning, CommsTick not set.\n");
	}

	SetAppFreq(appTick);
	SetCommsFreq(commsTick);

	DoRegistrations();

	return true;
}

void MOOSNode::DoRegistrations()
{
	MOOSTrace("Registering messages.\n");
	vector<shared_ptr<MsgContainer>>::iterator it;
	for ( it = _msgVec.begin() ; it < _msgVec.end() ; it++ )
	{
		m_Comms.Register(it->get()->moosName(),0);
		RCLCPP_INFO(_node->get_logger(), "Registering for %s\n", it->get()->moosName().c_str());

		char buf[512];
		sprintf(buf,"Subscribing to %s\n",it->get()->moosName().c_str());
		MOOSTrace(buf);
	}
}
