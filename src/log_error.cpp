#include "log_error.hpp"
#include <iostream>

void
media_finder::log_error(const std::string& msg)
{
  std::lock_guard lock(get_log_mutex());
  std::cerr << "Error: " << msg << std::endl;
}
