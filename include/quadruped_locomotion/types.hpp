#pragma once

#include <string>
#include <array>

namespace quadruped_locomotion::types
{

  struct QuadrupedTopology
  {
    static constexpr size_t NumLegs = 4;
    static constexpr size_t NumJointsPerLeg = 3;
    static constexpr size_t NumActuators =
      NumLegs * NumJointsPerLeg;
  };

  enum JointEnum : std::size_t {
    HAA = 0,
    HFE = 1,
    KFE = 2,
  };

  enum LegEnum : std::size_t {
    LF = 0,
    LH = 1,
    RF = 2,
    RH = 3,
  };

  struct JointConfig {
    std::string name = "";
  };

  struct JointState {
    double position = 0.0;
  };

  struct JointCommand {
    double position = 0.0;
  };

  template <typename JointT>
  struct Leg {
    std::array<JointT,QuadrupedTopology::NumJointsPerLeg> joints;
    JointT& operator[](JointEnum joint_enum){
      return joints[static_cast<std::size_t>(joint_enum)];
    }
    const JointT& operator[](JointEnum joint_enum) const {
      return joints[static_cast<std::size_t>(joint_enum)];
    }
  };

  template <typename JointT>
  struct QuadrupedJointT {

    std::array<Leg<JointT>,QuadrupedTopology::NumLegs> legs;

    Leg<JointT>& operator[](LegEnum leg_enum){
      return legs[static_cast<std::size_t>(leg_enum)];
    }
    const Leg<JointT>& operator[](LegEnum leg_enum) const {
      return legs[static_cast<std::size_t>(leg_enum)];
    }
  };

  enum AxisEnum : std::size_t {
    X = 0,
    Y = 1,
    Z = 2,
  };

  enum VectorEnum : std::size_t {
    LINEAR = 0,
    ANGULAR = 1
  };

  struct AxisConfig {
    std::string name = "";
  };

  struct AxisTwist{
    double velocity = 0.0;
  };

  template <typename AxisT>
  struct VectorCommand {
 
    std::array<AxisT,3> axes;

    AxisT& operator[](AxisEnum axes_enum) {
      return axes[static_cast<std::size_t>(axes_enum)];
    }

    const AxisT& operator[](AxisEnum axes_enum) const {
      return axes[static_cast<std::size_t>(axes_enum)];
    }

  };

  template <typename AxisT>
  struct QuadrupedAxisT {

    std::array<VectorCommand<AxisT>,2> vectors;

    VectorCommand<AxisT>& operator[](VectorEnum vector_enum) {
      return vectors[static_cast<std::size_t>(vector_enum)];
    }

    const VectorCommand<AxisT>& operator[](VectorEnum vector_enum) const {
      return vectors[static_cast<std::size_t>(vector_enum)];
    }

  };

  // struct QuadrupedConfig{
  //   QuadrupedAxisT<AxisConfig> command;
  //   QuadrupedJointT<JointConfig> state;
  // };
  
  struct QuadrupedSettings{
    std::string config_path;
    std::string urdf_path;
  };

  using QuadrupedTwist = QuadrupedAxisT<AxisTwist>;
  using QuadrupedCommand = QuadrupedJointT<JointCommand>;
  using QuadrupedState = QuadrupedJointT<JointState>;


}