#include <iostream>

#include "quadruped_locomotion/core/system.hpp"

int main(int argc, char** argv)
{
  core::System system(argc, argv);

  system.startBackgroundSpin();
  system.wait();

  return 0;
}


