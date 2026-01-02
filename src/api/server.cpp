
#include "quadruped_locomotion/api/server.hpp"

namespace quadruped_locomotion::api
{

using namespace quadruped_locomotion::types;

Server::Server(){

  state_publisher_.create_publisher("quadruped_locomotion_state");

  command_subscriber_.create_subscriber(
    "quadruped_locomotion_command",
    [this](const std::array<double,2>& data_array) {
      this->commandCallback(data_array);
    }
  );

}

void Server::spinOnce(){
  
  if (!background_spin_online_)
    spinOnceInternal();
  else
    throw std::logic_error("spinOnce() not allowed when using background spin");
  
}

void Server::startBackgroundSpin(){

  if (background_spin_online_) {
      throw std::logic_error("Background spin already running");
  }

  background_spin_online_ = true;

  transport_thread_ = std::jthread(
    [this](std::stop_token st){
      this->spin(st, 1000);
    });

}

void Server::stopBackgroundSpin(){

  if (transport_thread_.joinable()){

    transport_thread_.request_stop();
    transport_thread_.join();
  }

  background_spin_online_ = false;
}

QuadrupedTwist Server::getQuadrupedTwist()
{
  std::lock_guard<std::mutex> lock(command_mutex);

  if (command_array_received_)
  {
    //quadruped_locomotion::utils::print_array("Received: ", command_array_);
    quadruped_twist_[LINEAR][X].velocity = command_array_[0];
    quadruped_twist_[ANGULAR][Z].velocity = command_array_[1];
  }

  return quadruped_twist_; 
}

void Server::setQuadrupedStates(QuadrupedState quadruped_state)
{

  int index = 0;

  for (LegEnum leg: {LF, LH, RF, RH}){
    
      for (JointEnum joint: {HAA, HFE, KFE}){
        state_array_[index] = quadruped_state[leg][joint].position;
        index++;
      }
    }

  if (state_publisher_.publish(state_array_))
  {
    //quadruped_locomotion::utils::print_array("Sent: ", state_array_);
  }

}

void Server::commandCallback(const std::array<double,2>& data_array)
{
    std::lock_guard<std::mutex> lock(command_mutex);
    command_array_ = data_array;
    command_array_received_ = true;
}

void Server::spinOnceInternal(){

  command_subscriber_.spin_once();

}

void Server::spin(std::stop_token st, double frequency){

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
