#ifndef _ROS_H_
#define _ROS_H_

#include "ros/node_handle.h"
#include "embedded_linux_hardware.h"

namespace ros
{
typedef NodeHandle_<EmbeddedLinuxHardware> NodeHandle;
}

#endif
