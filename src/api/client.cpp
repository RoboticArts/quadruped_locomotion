
#include "quadruped_locomotion/api/client.hpp"

namespace quadruped_locomotion::api
{

using namespace quadruped_locomotion::types;

Client::Client(){

  command_publisher_.create_publisher("quadruped_locomotion_command");

  state_subscriber_.create_subscriber(
    "quadruped_locomotion_state",
    [this](const std::array<double,12>& data_array) {
      this->stateCallback(data_array);
    }
  );

}

void Client::spinOnce(){
  
  if (!background_spin_online_)
    spinOnceInternal();
  else
    throw std::logic_error("spinOnce() not allowed when using background spin");
  
}

void Client::startBackgroundSpin(){

  if (background_spin_online_) {
      throw std::logic_error("Background spin already running");
  }

  background_spin_online_ = true;

  transport_thread_ = std::jthread(
    [this](std::stop_token st){
      this->spin(st, 1000);
    });

}

void Client::stopBackgroundSpin(){

  if (transport_thread_.joinable()){

    transport_thread_.request_stop();
    transport_thread_.join();
  }

  background_spin_online_ = false;
}

void Client::setQuadrupedTwist(QuadrupedTwist quadruped_twist)
{

  command_array_[0] = quadruped_twist[LINEAR][X].velocity;
  command_array_[1] = quadruped_twist[ANGULAR][X].velocity;

  if (command_publisher_.publish(command_array_))
  {
    //quadruped_locomotion::utils::print_array("Sent: ", command_array_);
  }
  
}

QuadrupedState Client::getQuadrupedStates()
{

  std::lock_guard<std::mutex> lock(state_mutex);

  if (state_array_received_)
  {
    //quadruped_locomotion::utils::print_array("Received: ", state_array_);
    
    int index = 0;

    for (LegEnum leg: {LF, LH, RF, RH}){
    
      for (JointEnum joint: {HAA, HFE, KFE}){
        quadruped_state_[leg][joint].position = state_array_[index];
        index++;
      }
    }

  }

  return quadruped_state_;
}

void Client::stateCallback(const std::array<double,12>& data_array)
{
    std::lock_guard<std::mutex> lock(state_mutex);
    state_array_ = data_array;
    state_array_received_ = true;
}

void Client::spinOnceInternal()
{

  state_subscriber_.spin_once();

}

void Client::spin(std::stop_token st, double frequency){

  using clock = std::chrono::steady_clock;
  auto period = std::chrono::duration_cast<clock::duration>(
      std::chrono::duration<double>(1.0 / frequency)
  );
  auto next = clock::now();

  while(!st.stop_requested()){

    next += period;
    spinOnceInternal();
    std::this_thread::sleep_until(next);

  }

}

}

