#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <format>
#include <iostream>

namespace settings
{

class ArgumentParser
{
  public:

    ArgumentParser(const std::string& program_name);

    void add_argument(std::string& reference, std::string name, 
                      std::string description, std::string default_value);
    void parse(int argc, char** argv);
  
  private:

    const std::string program_name_;

    struct UserArgument
    {
      std::string name;
      std::string value;
    };

    struct ProgramArgument
    {
      std::reference_wrapper<std::string> reference;
      std::string name;
      std::string description;
      std::string default_value;
    };

    std::vector<UserArgument> user_args_;
    std::vector<ProgramArgument> program_args_;

    std::vector<UserArgument> getUserArguments(int argc, char** argv);
    void print_help() const;
};

}