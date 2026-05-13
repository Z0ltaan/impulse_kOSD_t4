#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <filesystem>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>

using json = nlohmann::json;

namespace media_finder
{
  void print_help_message(std::ostream& out)
  {
    out << "usage: media_finder <directory_path> <timeout_in_seconds>\n";
  }

  std::filesystem::path get_home_path()
  {
    const char* home = std::getenv("HOME");
    return std::filesystem::path{ home };
  }

  class directory_entries_collector
  {
  public:
    directory_entries_collector() : categorized_media_{} { clear(); }

    void operator()(const std::filesystem::directory_entry& entry)
    {
      if (!entry.is_regular_file())
      {
        return;
      }

      try
      {
        categorized_media_[extension_type_mapping.at(
                             entry.path().extension().c_str())]
          .push_back(entry.path().string());
      }
      catch (...)
      {}
    }

    void clear()
    {
      categorized_media_ = { { "audio", json::array() },
                             { "video", json::array() },
                             { "images", json::array() } };
    }

    std::string to_json() const { return categorized_media_.dump(); };

  private:
    json categorized_media_;

    const inline static std::map< std::string_view, std::string_view >
      extension_type_mapping = {
        { ".wav", "audio" },   { ".aac", "audio" },   { ".wma", "audio" },
        { ".flac", "audio" },  { ".ogg", "audio" },   { ".mp3", "audio" },
        { ".alac", "audio" },  { ".aiff", "audio" },  { ".mp4", "video" },
        { ".mov", "video" },   { ".mkv", "video" },   { ".webm", "video" },
        { ".wmv", "video" },   { ".avi", "video" },   { ".jpeg", "images" },
        { ".jpg", "images" },  { ".png", "images" },  { ".webp", "images" },
        { ".gif", "images" },  { ".avif", "images" }, { ".svg", "images" },
        { ".tiff", "images" }, { ".heif", "images" }, { ".heic", "images" },
      };
  };

}
int
main(int argc, char** argv)
{
  try
  {
    if (argc != 3)
    {
      media_finder::print_help_message(std::cerr);
      throw std::runtime_error("wrong amount of arguments");
    }

    std::filesystem::path directory_to_monitor(argv[1]);

    if (!std::filesystem::is_directory(directory_to_monitor))
    {
      throw std::runtime_error("provided path is not a directory");
    }

    std::chrono::seconds timeout(std::stoul(argv[2]));

    std::cout << directory_to_monitor << ' ' << timeout.count() << '\n';

    media_finder::directory_entries_collector collector;

    auto begin =
      std::filesystem::recursive_directory_iterator{ directory_to_monitor };
    auto end = std::filesystem::recursive_directory_iterator{};
    std::for_each(begin, end, std::ref(collector));

    std::cout << collector.to_json() << '\n';
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
}
