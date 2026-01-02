#pragma once

#include <filesystem>

#include "quadruped_locomotion/types.hpp"
#include "quadruped_locomotion/settings/argument_parser.hpp"

namespace settings
{

using namespace quadruped_locomotion::types;
namespace fs = std::filesystem;

class Loader {

  public:

    QuadrupedSettings getQuadrupedSettings (int argc, char** argv);
  
  private:
    
    // Get binary path
    fs::path default_directory_path();
};

}