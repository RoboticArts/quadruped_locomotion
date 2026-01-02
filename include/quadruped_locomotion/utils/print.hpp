#pragma once
#include <iostream>

#include "quadruped_locomotion/types.hpp"

namespace quadruped_locomotion::utils{

    using namespace quadruped_locomotion::types;

    /// @brief Gets the current time since epoch in the specified duration type.
    /// @tparam T Duration type (e.g., std::chrono::milliseconds).
    /// @return Current time since epoch as an integer count of the specified duration type.
    template <typename T, size_t N>
    inline void print_array(std::string msg, std::array<T,N> data_array)
    {

        std::cout << msg;
        for (auto data: data_array){
          std::cout << data << " ";
        }
        std::cout << std::endl;

    }

    /// @brief Gets the current time since epoch in the specified duration type.
    /// @tparam T Duration type (e.g., std::chrono::milliseconds).
    /// @return Current time since epoch as an integer count of the specified duration type.
    inline void print_quadruped_twist(std::string_view msg, const QuadrupedTwist& quadruped_twist){

      std::cout << msg << "\n";
      std::cout << "Linear X: " << quadruped_twist[LINEAR][X].velocity
                << "Angular Y: " << quadruped_twist[ANGULAR][Z].velocity
                << "\n"; 

    }

    /// @brief Gets the current time since epoch in the specified duration type.
    /// @tparam T Duration type (e.g., std::chrono::milliseconds).
    /// @return Current time since epoch as an integer count of the specified duration type.
    inline void print_quadruped_state(std::string_view msg, const QuadrupedState& quadruped_state){

      static constexpr std::array<std::string_view,4> leg_name{"LF", "LH", "RF", "RH"};
      static constexpr std::array<std::string_view,3> joint_name{"HAA", "HFE", "KFE"};

      std::cout << msg << "\n";

      for (LegEnum leg: {LF, LH, RF, RH}){

        for (JointEnum joint: {HAA, HFE, KFE}){

          std::cout << leg_name[static_cast<std::size_t>(leg)] << "_" 
                    << joint_name[static_cast<std::size_t>(joint)] << ": "
                    << quadruped_state[leg][joint].position << " ";
        }

        std::cout << "\n";
      }
    }
  
};