#ifndef HANDLE_CLI_OPTIONS_HPP
#define HANDLE_CLI_OPTIONS_HPP

#include <CLI/App.hpp>
#include "config.hpp"

namespace media_finder
{
  config handle_cli_options(int argc,
                            char** argv,
                            CLI::App& arguments_controller);
}

#endif
