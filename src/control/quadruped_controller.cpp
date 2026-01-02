#include "quadruped_locomotion/control/quadruped_controller.hpp"

namespace control
{

QuadrupedCommand 
QuadrupedController::
quadrupedControl(QuadrupedTwist quadruped_twist)
{
  (void)quadruped_twist;
  return quadruped_command_;
}

void QuadrupedController::computeController()
{
  pushUps();
}

QuadrupedCommand QuadrupedController::pushUps()
{

  using namespace quadruped_locomotion::types;

  if (elapsed_push_ups > 3000)
  {
    elapsed_push_ups.reset();
    
    if (state)
    {
      std::cout << "Push" << std::endl;

      quadruped_command_[LF][HAA].position = 0.0;
      quadruped_command_[LF][HFE].position = 0.82;
      quadruped_command_[LF][KFE].position = -1.22;
      
      quadruped_command_[LH][HAA].position = 0.0;
      quadruped_command_[LH][HFE].position = -0.82;
      quadruped_command_[LH][KFE].position = 1.22;

      quadruped_command_[RF][HAA].position = 0.0;
      quadruped_command_[RF][HFE].position = 0.82;
      quadruped_command_[RF][KFE].position = -1.22;

      quadruped_command_[RH][HAA].position = 0.0;
      quadruped_command_[RH][HFE].position = -0.82;
      quadruped_command_[RH][KFE].position = 1.22;

      state = false;
    }
    else
    {
      std::cout << "Up" << std::endl;

      quadruped_command_[LF][HAA].position = 0.0;
      quadruped_command_[LF][HFE].position = 1.2; 
      quadruped_command_[LF][KFE].position = -1.82;
      
      quadruped_command_[LH][HAA].position = 0.0;
      quadruped_command_[LH][HFE].position = -1.2;
      quadruped_command_[LH][KFE].position = 1.82;

      quadruped_command_[RF][HAA].position = 0.0;
      quadruped_command_[RF][HFE].position = 1.2;
      quadruped_command_[RF][KFE].position = -1.82;

      quadruped_command_[RH][HAA].position = 0.0;
      quadruped_command_[RH][HFE].position = -1.2;
      quadruped_command_[RH][KFE].position = 1.82;

      state = true;
    }
  }

  return quadruped_command_;
}


void QuadrupedController::spinOnceInternal()
{
  computeController();
}

}