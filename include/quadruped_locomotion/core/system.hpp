#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <string>

#include "quadruped_locomotion/types.hpp"
#include "quadruped_locomotion/utils/time.hpp"
#include "quadruped_locomotion/utils/print.hpp"

#include "quadruped_locomotion/core/thread_component.hpp"
#include "quadruped_locomotion/api/server.hpp"
#include "quadruped_locomotion/settings/loader.hpp"
#include "quadruped_locomotion/control/quadruped_controller.hpp"
#include "quadruped_locomotion/driver/quadruped_driver.hpp"

namespace core
{

using namespace quadruped_locomotion;

class System : public core::ThreadComponent
{
  public:

    System(int argc, char** argv);
    void wait();
  
  private:

    void spinOnceInternal() override;
    void update();

    types::QuadrupedSettings quadruped_settings;
    types::QuadrupedTwist quadruped_twist;
    types::QuadrupedCommand quadruped_command;
    types::QuadrupedState quadruped_state;

    settings::Loader loader;
    api::Server server;
    control::QuadrupedController controller;
    driver::QuadrupedDriver driver;

};

}