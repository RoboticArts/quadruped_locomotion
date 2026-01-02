#pragma once

#include <iostream>
#include <optional>
#include <thread>
#include <atomic>
#include <format>
#include <filesystem>
#include <yaml-cpp/yaml.h>

#include "quadruped_locomotion/types.hpp"
#include "quadruped_locomotion/utils/yaml.hpp"
#include "quadruped_locomotion/core/thread_component.hpp"
#include "quadruped_locomotion/driver/gazebo_driver.hpp"

namespace driver 
{

namespace fs = std::filesystem;

using QuadrupedCommand = quadruped_locomotion::types::QuadrupedCommand;
using QuadrupedState = quadruped_locomotion::types::QuadrupedState;
using DriverI = driver::GazeboDriver;

class QuadrupedDriver : public core::ThreadComponent {

  public:

    void init(fs::path config_path);
    void setQuadrupedCommand(QuadrupedCommand quadruped_command);
    QuadrupedState getQuadrupedState();

  private:
    
    void spinOnceInternal() override;

    std::string to_yaml_leg_key(LegEnum leg);
    std::string to_yaml_joint_key(JointEnum joint);

    DriverI quadruped_driver_interface_;

};

}

//using JointMotorDriver = std::optional<driver::GazeboMotorDriver>;
//using QuadrupedMotorDriver = quadruped_locomotion::types::QuadrupedJointT<JointMotorDriver>;

// using LegEnum = quadruped_locomotion::types::LegEnum;
// using JointEnum = quadruped_locomotion::types::JointEnum;