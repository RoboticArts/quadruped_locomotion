#pragma once

#include <iostream>
#include <filesystem>
#include <array>
#include <unordered_map>
#include <atomic>

#include <gz/transport/Node.hh>
#include <gz/msgs/double.pb.h>
#include <gz/msgs/actuators.pb.h>
#include <gz/msgs/model.pb.h>

#include "quadruped_locomotion/utils/yaml.hpp"
#include "quadruped_locomotion/driver/driver_interface.hpp"

namespace driver
{

using QuadrupedCommand = quadruped_locomotion::types::QuadrupedCommand;
using QuadrupedState = quadruped_locomotion::types::QuadrupedState;

using JointCommand = quadruped_locomotion::types::JointCommand;
using JointState = quadruped_locomotion::types::JointState;

using QuadrupedTopology = quadruped_locomotion::types::QuadrupedTopology;
using LegEnum = quadruped_locomotion::types::LegEnum;
using JointEnum = quadruped_locomotion::types::JointEnum;

namespace fs = std::filesystem;

class GazeboDriver : public DriverInterface
{

  public:

    void init(std::string config) override;
    void setCommand(QuadrupedCommand quadruped_command) override;
    QuadrupedState getState() override;
    void writeCommand() override;
    void readState() override;

  private:

    void jointStateCb(const gz::msgs::Model &msg);
    //void initializeFromModel(const gz::msgs::Model &msg);

    std::string to_yaml_joint_key(JointEnum joint);
    std::string to_yaml_leg_key(LegEnum leg);

    std::mutex state_mutex_;
    std::mutex command_mutex_;

    std::string topic_state_name_;
    std::string topic_command_name_;

    gz::transport::Node gz_node_;
    gz::transport::Node::Publisher actuator_pub_;  

    struct DriverIndex{
      std::string name;
      LegEnum leg;
      JointEnum joint;
    };

    std::vector<DriverIndex> quadruped_gazebo_write_index_;
    std::unordered_map<std::string, double> gazebo_read_;

    struct Driver
    {
      std::string joint_name = "";
      int write_id = -1;
      int read_id = -1;
      double position_command = 0.0;
      double position_state = 0.0;
    };

    using QuadrupedGazebo = 
      quadruped_locomotion::types::QuadrupedJointT<Driver>;

    QuadrupedGazebo quadruped_gazebo_;

    int joint_size_{0};
    std::vector<gz::msgs::Joint> joint_array_gz_;

    std::once_flag model_init_flag_;
    
    std::atomic<bool> model_initialized_{false};

};

}