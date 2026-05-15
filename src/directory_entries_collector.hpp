#ifndef DIRECTORY_ENTRY_COLLECTOR_HPP
#define DIRECTORY_ENTRY_COLLECTOR_HPP

#include <filesystem>
#include <map>
#include <nlohmann/json.hpp>
#include <string>

namespace media_finder
{
  class directory_entries_collector
  {
  public:
    using json = nlohmann::json;

    directory_entries_collector();

    void operator()(const std::filesystem::directory_entry& entry);

    void clear();

    std::string to_json_string() const;

  private:
    json categorized_media_;

    static const std::map< std::string, std::string >
      extension_category_mapping;
  };
}

#endif
