#include "handle_cli_options.hpp"
#include <CLI/App.hpp>
#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include "config.hpp"

static std::filesystem::path
get_home_path()
{
  const char* home = std::getenv("HOME");
  if (!home)
  {
    throw std::runtime_error("cant access environment variable HOME");
  }
  return std::filesystem::path{ home };
}

media_finder::config
media_finder::handle_cli_options(int argc,
                                 char** argv,
                                 CLI::App& arguments_controller)
{

  arguments_controller.name("media_finder");
  arguments_controller.description("Small (media)filewatcher server");

  argv = arguments_controller.ensure_utf8(argv);

  size_t interval = 30;
  arguments_controller.add_option(
    "-i,--interval",
    interval,
    "Set polling interval in seconds (defaults to 30)");

  std::string directory_path;
  arguments_controller
    .add_option("-p,--path",
                directory_path,
                "Set directory to monitor (defaults to $HOME)")
    ->check(CLI::ExistingDirectory);

  arguments_controller.parse(argc, argv);

  config config{};

  if (directory_path == "")
  {
    config.directory_to_monitor = get_home_path();
  }
  // else if (!std::filesystem::is_directory(directory_path))
  // {
  //   throw std::runtime_error("provided path is not a directory");
  // }
  else
  {
    config.directory_to_monitor = directory_path;
  }
  config.interval = std::chrono::seconds{ interval };

  return config;
}
