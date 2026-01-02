#pragma once
#include <iostream>
#include <string>
#include <filesystem>
#include <format>
#include <yaml-cpp/yaml.h>

#include "quadruped_locomotion/types.hpp"

namespace quadruped_locomotion::utils{

  namespace fs = std::filesystem;

  /// @brief Gets the current time since epoch in the specified duration type.
  /// @tparam T Duration type (e.g., std::chrono::milliseconds).
  /// @return Current time since epoch as an integer count of the specified duration type.
  inline YAML::Node load_yaml(fs::path yaml_path, std::string yaml_root, std::string err_msg)
  {
    if (!fs::exists(yaml_path)) 
    {
        throw std::runtime_error(
            std::format("[YamlLoader] File does not exist: {} {}", 
            yaml_path.string(), err_msg)
        );
    }
    else
    {
      std::cout << "[YamlLoader] yaml_path: " 
                << yaml_path << '\n';
    }

    YAML::Node root = YAML::LoadFile(yaml_path.string());

    return root[yaml_root];
  }

}
