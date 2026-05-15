#include "collect_data.hpp"
#include <algorithm>
#include "directory_entries_collector.hpp"

std::string
media_finder::collect_data(const std::filesystem::path& directory_to_monitor)
{
  auto begin = std::filesystem::recursive_directory_iterator{
    directory_to_monitor,
    std::filesystem::directory_options::skip_permission_denied
  };

  media_finder::directory_entries_collector collector;
  std::for_each(begin,
                std::filesystem::recursive_directory_iterator{},
                std::ref(collector));
  return collector.to_json_string();
}
