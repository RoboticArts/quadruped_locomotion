#pragma once

#include <string>

#include "quadruped_locomotion/types.hpp"

namespace driver 
{

using QuadrupedCommand = quadruped_locomotion::types::QuadrupedCommand;
using QuadrupedState = quadruped_locomotion::types::QuadrupedState;

class DriverInterface
{
  public:

    DriverInterface() = default;
    virtual ~DriverInterface() = default;

    virtual void init(std::string config) = 0;
    virtual void setCommand(QuadrupedCommand quadruped_command) = 0;
    virtual QuadrupedState getState() = 0;
    virtual void writeCommand() = 0;
    virtual void readState() = 0;

};

}
