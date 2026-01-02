#include "quadruped_locomotion/driver/gazebo_driver.hpp"

namespace driver
{

using namespace quadruped_locomotion::types;

void GazeboDriver::init(std::string config){

  fs::path driver_config_path = config;

  const YAML::Node& params = quadruped_locomotion::utils::load_yaml(
    driver_config_path,
    "quadruped_drivers_config",
    "[YamlLoader] File does not exist: {}. Use --driver-config to set a valid path."
  );

  topic_state_name_ = "/world/default/model/robot/anymal/joint_state"; //params["gazebo"]["read"].as<std::string>();
  topic_command_name_ = "/actuators"; //params["gazebo"]["write"].as<std::string>();

  int max_write_id = 0;

  for (LegEnum leg: {LF, LH, RF, RH}){
    for (JointEnum joint: {HAA, HFE, KFE})
    {
      auto leg_key = to_yaml_leg_key(leg);
      auto joint_key = to_yaml_joint_key(joint);
  
      std::string joint_name = params[leg_key][joint_key]["name"].as<std::string>(); 
      int write_id = params[leg_key][joint_key]["id"].as<int>();

      quadruped_gazebo_[leg][joint].joint_name = joint_name;
      quadruped_gazebo_[leg][joint].write_id = write_id;
      
      if (write_id >= max_write_id)
      {
        max_write_id = write_id;
        quadruped_gazebo_write_index_.resize(max_write_id + 1);
      }

      quadruped_gazebo_write_index_[write_id] = {joint_name, leg, joint};
      //quadruped_gazebo_read_index_[joint_name] = {leg, joint};

    }
  }

  actuator_pub_ = gz_node_.Advertise<gz::msgs::Actuators>(topic_command_name_);
  gz_node_.Subscribe(topic_state_name_, &GazeboDriver::jointStateCb, this);
  
}

void GazeboDriver::setCommand(QuadrupedCommand quadruped_command)
{
  for (LegEnum leg: {LF, LH, RF, RH}){
    for (JointEnum joint: {HAA, HFE, KFE})
    {
      quadruped_gazebo_[leg][joint].position_command =
        quadruped_command[leg][joint].position;
    }
  }
  // std::lock_guard<std::mutex> lock(command_mutex_);
  // joint_command_ = command;
}

QuadrupedState GazeboDriver::getState()
{
  std::lock_guard<std::mutex> lock(state_mutex_);

  QuadrupedState quadruped_state;

  for (LegEnum leg: {LF, LH, RF, RH}){
    for (JointEnum joint: {HAA, HFE, KFE})
    {
      quadruped_state[leg][joint].position =
        quadruped_gazebo_[leg][joint].position_state;
    } 
  }

  //std::cout << "D2: " << quadruped_gazebo_[LF][HFE].position_state << std::endl;
  return quadruped_state;
}

void GazeboDriver::writeCommand()
{
  gz::msgs::Actuators msg;

  {
    std::lock_guard<std::mutex> lock(command_mutex_);

    for (const auto& index: quadruped_gazebo_write_index_)
    {
      msg.add_position(
        quadruped_gazebo_[index.leg][index.joint].position_command
      );
    }
  }

  actuator_pub_.Publish(msg);
}

void GazeboDriver::readState()
{
  std::lock_guard<std::mutex> lock(state_mutex_);

  //std::cout << "D1: " << quadruped_gazebo_[LF][HFE].position_state << std::endl;

  for (LegEnum leg: {LF, LH, RF, RH}){
    for (JointEnum joint: {HAA, HFE, KFE})
    {
      const std::string& name = quadruped_gazebo_[leg][joint].joint_name;
      
      if (gazebo_read_.contains(name))
      {
        quadruped_gazebo_[leg][joint].position_state = 
          gazebo_read_.at(name);

        // std::cout << "G: " << gazebo_read_.at(name) << std::endl;
        // std::cout << "Q: " << quadruped_gazebo_[leg][joint].position_state << std::endl;
      }
    }
  }
}

void GazeboDriver::jointStateCb(const gz::msgs::Model &msg)
{
  std::lock_guard<std::mutex> lock(state_mutex_);

    for (int i = 0; i < msg.joint_size(); ++i)
    {
      if (msg.joint(i).has_axis1())
      {
        gazebo_read_[msg.joint(i).name()] = 
          msg.joint(i).axis1().position();
      }
    }
}


// void GazeboDriver::initializeFromModel(const gz::msgs::Model &msg)
// {
//   joint_size_ = msg.joint_size();
//   joint_array_gz_.resize(joint_size_);

//   for (LegEnum leg: {LF, LH, RF, RH}){
//     for (JointEnum joint: {HAA, HFE, KFE})
//     {
//       bool found = false;
//       const std::string& name = quadruped_gazebo_[leg][joint].joint_name;

//       for (int i = 0; i <joint_size_; ++i)
//       {
//         std::cout << msg.joint(i).name() << std::endl; 
        
//         if (name == msg.joint(i).name())
//         {
//           quadruped_gazebo_[leg][joint].read_id = i;
//           found = true;
//           break;
//         }
//       }

//       if (!found){
//         throw std::logic_error(
//           std::format("[GazeboDriver]: Joint '{}' not found in gazebo joints", name)
//         );
//       }
//     }
//   }
// }


std::string GazeboDriver::to_yaml_leg_key(LegEnum leg)
{
  switch (leg) {
    case LegEnum::LF: return "lf_drivers";
    case LegEnum::LH: return "lh_drivers";
    case LegEnum::RF: return "rf_drivers";
    case LegEnum::RH: return "rh_drivers";
  }
  throw std::logic_error("Invalid LegEnum");

}

std::string GazeboDriver::to_yaml_joint_key(JointEnum joint)
{
  switch (joint) {
    case JointEnum::HAA: return "haa_driver";
    case JointEnum::HFE: return "hfe_driver";
    case JointEnum::KFE: return "kfe_driver";
  }
  throw std::logic_error("Invalid JointEnum");
}

}