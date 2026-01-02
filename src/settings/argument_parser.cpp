#include "quadruped_locomotion/settings/argument_parser.hpp"

namespace settings
{

ArgumentParser::ArgumentParser(const std::string& program_name)
  : program_name_(program_name) {};

void ArgumentParser::add_argument(
  std::string& reference, std::string name, std::string description,
  std::string default_value)
{
  reference = default_value;

  program_args_.emplace_back(
    ProgramArgument{
      .reference = reference,
      .name = std::move(name),
      .description = std::move(description),
      .default_value = std::move(default_value)
    }
  );
}

void ArgumentParser::parse(int argc, char** argv)
{

  user_args_ = getUserArguments(argc, argv);

  for (auto& program_arg: program_args_)
  {
    for (auto& user_arg: user_args_)
    {
        if (program_arg.name == user_arg.name)
        {
          program_arg.reference.get() = user_arg.value;
          break;
        }
    }
  }
}

std::vector<ArgumentParser::UserArgument> 
ArgumentParser::getUserArguments(int argc, char** argv)
{
  std::vector<UserArgument> user_args;
  std::vector<std::string> args (argv + 1, argv + argc);

  for (auto it = args.begin(); it != args.end(); ++it)
  {
    const std::string& arg = *it;
    
    // Looks for argument name --param
    if (arg.starts_with("--"))
    {
      auto next = std::next(it);

      if (arg == "--help")
      {
        print_help();
        std::exit(0);
      }
      // Check if argument name is not the last argument
      else if (next != args.end()) 
      {
        // Check that next argument is an valid argument value:
        //     --param value (value do no contain --prefix)
        if (!next->starts_with("--"))
        {
          // Add user arg name and value
          user_args.emplace_back(
            UserArgument{
              .name = arg,
              .value = *next
              }
          );

          ++it; // Jump to next arg
        }
      }
      else
      {
        // TODO(robert): Manage flags. p.e: --enable 
        // throw std::invalid_argument(
        //     std::format("[ArgumentParser]: Missing value for {}", arg)
        // );
      }
    }
  }

  return user_args;
}

void ArgumentParser::print_help() const
{
  std::cout << "Usage:\n"
            << "  " << program_name_ << " [options]\n\n";

  std::cout << "Options:\n";

  for (const auto& arg : program_args_)
  {
    std::cout << "  " << arg.name << " <value>\n"
              << "      " << arg.description << "\n";
  }
}


}