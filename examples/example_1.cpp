#include <thread>
#include <chrono>

#include <quadruped_locomotion/types.hpp>
#include <quadruped_locomotion/api/client.hpp>

using namespace quadruped_locomotion;

int main(){

  api::Client client;
  types::QuadrupedTwist quadruped_command;
  types::QuadrupedState quadruped_state;
  
  client.startBackgroundSpin();

  while(true){
    
    client.getQuadrupedStates();

    quadruped_command[types::LINEAR][types::X].velocity += 1.0;
    quadruped_command[types::ANGULAR][types::Z].velocity = 2.0;

    client.setQuadrupedTwist(quadruped_command);

    std::cout << "Sent" << std::endl;
    std::cout << " X: " << quadruped_command[types::LINEAR][types::X].velocity 
              << " Y: " << quadruped_command[types::ANGULAR][types::Z].velocity 
              << std::endl; 

    quadruped_state = client.getQuadrupedStates();

    std::cout << "Received" << std::endl;
    std::cout << " LF_HAA: " << quadruped_state[types::LF][types::HAA].position 
              << " LF_HFE: " << quadruped_state[types::LF][types::HFE].position
              << " LF_KFE: " << quadruped_state[types::LF][types::KFE].position
              << std::endl; 

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return 1;
}