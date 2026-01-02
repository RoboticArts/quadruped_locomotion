#pragma once

#include <zmq.hpp>
#include <array>
#include <chrono>
#include <thread>
#include <iostream>

#include "quadruped_locomotion/types.hpp"
#include "quadruped_locomotion/api/transport.hpp"
#include "quadruped_locomotion/utils/print.hpp"

namespace quadruped_locomotion::api
{

using namespace quadruped_locomotion::types;

class Server {

  public:

    Server();

    QuadrupedTwist getQuadrupedTwist();
    void setQuadrupedStates(QuadrupedState quadruped_state);

    void startBackgroundSpin();
    void stopBackgroundSpin();
    void spinOnce();

  private:

    void spin(std::stop_token st, double frequency);
    void spinOnceInternal();
    void commandCallback(const std::array<double,2>& data_array);

    std::array<double,12> state_array_{0};
    std::array<double,2> command_array_{0};

    QuadrupedTwist quadruped_twist_;
  
    transport::Subscriber<double,2> command_subscriber_;
    transport::Publisher<double,12> state_publisher_;

    std::jthread transport_thread_;
    std::mutex command_mutex;

    std::atomic<bool> command_array_received_{false};

    std::atomic<bool> background_spin_online_{false};
};

}