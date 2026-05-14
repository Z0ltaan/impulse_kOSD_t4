#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <chrono>
#include <filesystem>

namespace media_finder
{
  struct config
  {
    std::filesystem::path directory_to_monitor;
    std::chrono::seconds interval;
  };
}

#endif
