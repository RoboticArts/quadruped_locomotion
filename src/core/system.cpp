#include "quadruped_locomotion/core/system.hpp"

namespace core
{

using namespace quadruped_locomotion;

System::System(int argc, char** argv)
{
  quadruped_settings = loader.getQuadrupedSettings(argc, argv);
  driver.init(quadruped_settings.config_path);

  driver.startBackgroundSpin();
  controller.startBackgroundSpin();
  server.startBackgroundSpin();
}

void System::update()
{
  quadruped_twist = server.getQuadrupedTwist();
  quadruped_command = controller.quadrupedControl(quadruped_twist);

  driver.setQuadrupedCommand(quadruped_command);

  auto quadruped_state = driver.getQuadrupedState();
  server.setQuadrupedStates(quadruped_state);
}

void System::spinOnceInternal()
{
  update();
}

void System::wait()
{
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

}