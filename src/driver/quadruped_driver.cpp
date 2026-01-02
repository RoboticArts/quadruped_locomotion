#include "quadruped_locomotion/driver/quadruped_driver.hpp"

namespace driver 
{

using namespace quadruped_locomotion::types;

void QuadrupedDriver::init(fs::path driver_config_path)
{
  quadruped_driver_interface_.init(driver_config_path);
}

void QuadrupedDriver::setQuadrupedCommand(QuadrupedCommand quadruped_command)
{
  quadruped_driver_interface_.setCommand(quadruped_command);
}


QuadrupedState QuadrupedDriver::getQuadrupedState()
{
  return quadruped_driver_interface_.getState();
}


void QuadrupedDriver::spinOnceInternal()
{
  quadruped_driver_interface_.readState();
  quadruped_driver_interface_.writeCommand();
}

}