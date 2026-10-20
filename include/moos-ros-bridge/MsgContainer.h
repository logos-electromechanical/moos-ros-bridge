//* The MOOS ROS Bridge
/**
 * 
 * @file
 *
 * @file
 *
 * @author Kevin DeMarco <kevin.demarco@gtri.gatech.edu>
 * @author Pierce Nichols <pierce@ladonrobotics.com>
 *
 * @version 2.0
 *
 * @date Oct 19 2026
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
#include <string>

using namespace std;

// The message direction for a particular message container
enum class BridgeDirection
{
	toROS,
	toMOOS
};

// Supported ROS types
enum class ROSType 
{
	Int32,
	Int64,
	Float32,
	Float64,
	String,
	UInt8Array,
	ByteArray,
	NONE
};

// Supported MOOS types
enum class MOOSType 
{
	DOUBLE,
	STRING,
	BINARY,
	NONE
};

// The intent here is that we can have a base class with some type erasure business going on but enough metadata to static_cast a pointer to an 
// instance. 
class MsgContainer
{
public:
	const string &moosName() { return _moosName; }
	const string &rosName() { return _rosName; }
	const MOOSType moosType() { return _moosType; }
	const ROSType rosType() { return _rosType; }
	const BridgeDirection dir() { return _dir; }

protected:
	MsgContainer(const string new_moosName, const string new_rosName, const MOOSType new_moosType, const ROSType new_rosType, const BridgeDirection dir) :
		_moosName(new_moosName), _rosName(new_rosName), _moosType(new_moosType), _rosType(new_rosType), _dir(dir)
	{}

	BridgeDirection _dir;
	
private:
	string _moosName;
	string _rosName;
	MOOSType _moosType;
	ROSType _rosType;
};

template<class T>
class MsgContainerPub : public MsgContainer
{
public:
	// from MOOS to ROS
	MsgContainerPub(const string new_moosName, const string new_rosName, const MOOSType new_moosType, const ROSType new_rosType, typename rclcpp::Publisher<T>::SharedPtr new_pub) :
		MsgContainer(new_moosName, new_rosName, new_moosType, new_rosType, BridgeDirection::toROS), pub(new_pub)
	{}
	~MsgContainerPub() {}

	typename rclcpp::Publisher<T>::SharedPtr pub;

};

template<class T>
class MsgContainerSub : public MsgContainer
{
public:

	// from ROS to MOOS
	MsgContainerSub(string new_moosName, string new_rosName, const MOOSType new_moosType, const ROSType new_rosType, typename rclcpp::Subscription<T>::SharedPtr new_sub)  :
		MsgContainer(new_moosName, new_rosName, new_moosType, new_rosType, BridgeDirection::toMOOS), sub(new_sub)
	{}
	~MsgContainerSub() {}

	typename rclcpp::Subscription<T>::SharedPtr sub;
};



