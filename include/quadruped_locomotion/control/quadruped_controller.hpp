#pragma once

#include <iostream>

#include "quadruped_locomotion/types.hpp"
#include "quadruped_locomotion/utils/time.hpp"

#include "quadruped_locomotion/core/thread_component.hpp"

namespace control
{

using QuadrupedTwist = quadruped_locomotion::types::QuadrupedTwist;
using QuadrupedCommand = quadruped_locomotion::types::QuadrupedCommand;
using QuadrupedState = quadruped_locomotion::types::QuadrupedState;

class QuadrupedController : public core::ThreadComponent
{
  public:

  QuadrupedCommand quadrupedControl(QuadrupedTwist quadruped_twist);

  private:

    void spinOnceInternal() override;
    void computeController();
    QuadrupedCommand pushUps();

    QuadrupedCommand quadruped_command_;

    quadruped_locomotion::utils::ElapsedMillis elapsed_push_ups;
    bool state{true};
  
};

}