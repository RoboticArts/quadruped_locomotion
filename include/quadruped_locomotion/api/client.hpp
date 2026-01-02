#pragma once

#include <zmq.hpp>
#include <array>
#include <chrono>
#include <thread>
#include <iostream>
#include <thread>

#include "quadruped_locomotion/types.hpp"
#include "quadruped_locomotion/api/transport.hpp"
#include "quadruped_locomotion/utils/print.hpp"

namespace quadruped_locomotion::api
{

using namespace quadruped_locomotion::types;

class Client {

  public:

    Client();

    void spinOnce();
    void setQuadrupedTwist(QuadrupedTwist quadruped_twist);
    QuadrupedState getQuadrupedStates();
    void startBackgroundSpin();
    void stopBackgroundSpin();

  private:

    void stateCallback(const std::array<double,12>& data_array);
    void spinOnceInternal();
    void spin(std::stop_token st, double frequency);

    std::array<double,2> base_command_{};
    std::array<double,12> joint_position_states_{};

    std::array<double,2> command_array_{0};
    std::array<double,12> state_array_{0};

    QuadrupedState quadruped_state_;
  
    transport::Publisher<double,2> command_publisher_;
    transport::Subscriber<double,12> state_subscriber_;

    std::jthread transport_thread_;
    std::mutex state_mutex;

    std::atomic<bool> state_array_received_{false};

    std::atomic<bool> background_spin_online_{false};

};

}