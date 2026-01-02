#include "quadruped_locomotion/settings/loader.hpp"

namespace settings
{

using namespace quadruped_locomotion::types;
namespace fs = std::filesystem;

QuadrupedSettings Loader::getQuadrupedSettings (int argc, char** argv)
{
  QuadrupedSettings quadruped_settings;

  ArgumentParser args("quadruped_locomotion");

  fs::path default_driver_config_path = default_directory_path() / "quadruped_drivers.yaml";
  args.add_argument(quadruped_settings.config_path, 
    "--driver-config", "Path to driver configuration file", default_driver_config_path);
  
  fs::path default_urdf_path = default_directory_path() / "quadruped.urdf";
  args.add_argument(quadruped_settings.urdf_path, 
    "--urdf", "Path to URDF file", default_urdf_path);

  args.parse(argc, argv);

  return quadruped_settings;
}



// Get binary path
fs::path Loader::default_directory_path()
{
  fs::path exe = fs::canonical("/proc/self/exe");

  fs::path exe_dir = exe.parent_path();        // <prefix>/bin
  fs::path prefix  = exe_dir.parent_path();    // <prefix>

  fs::path config =
      prefix / "share" / "quadruped_locomotion" / "config";

  return config;
}



}