#ifndef COLLECT_DATA_HPP
#define COLLECT_DATA_HPP

#include <filesystem>
#include <string>

namespace media_finder
{
  std::string collect_data(const std::filesystem::path& directory_to_monitor);

}
#endif
