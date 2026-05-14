#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <ostream>
#include <string>
#include <string_view>
#include <thread>
#include "CLI/CLI.hpp"
#include "config.hpp"
#include "handle_cli_options.hpp"

using json = nlohmann::json;

namespace media_finder
{
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
        categorized_media_[extension_category_mapping.at(
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

    std::string to_json_string() const { return categorized_media_.dump(); };

  private:
    json categorized_media_;

    const inline static std::map< std::string_view, std::string_view >
      extension_category_mapping = {
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
  class output_json
  {
  public:
    explicit output_json(const std::string& rhs) : mtx_(), data_(rhs) {}

    std::string get_data() const
    {
      std::lock_guard l{ mtx_ };
      return data_;
    }

    void set_data(std::string rhs)
    {
      std::lock_guard l{ mtx_ };
      data_ = std::move(rhs);
    }

  private:
    mutable std::mutex mtx_;
    std::string data_;
  };

  std::string collect_data(const std::filesystem::path& directory_to_monitor)
  {
    media_finder::directory_entries_collector collector;
    std::for_each(
      std::filesystem::recursive_directory_iterator{ directory_to_monitor },
      std::filesystem::recursive_directory_iterator{},
      std::ref(collector));
    return collector.to_json_string();
  }

}

int
main(int argc, char** argv)
{
  CLI::App arguments_handler;
  try
  {
    media_finder::config config =
      media_finder::handle_cli_options(argc, argv, arguments_handler);

    std::cout << "starting with directory = " << config.directory_to_monitor
              << ", interval = " << config.interval.count() << " seconds\n";

    auto end_time = std::chrono::steady_clock::now();

    auto output = std::make_shared< media_finder::output_json >("{}");

    // TODO: still synchronous; there needs to be an asynchronous traverse i think
    for (;;)
    {
      end_time += config.interval;

      output->set_data(
        std::move(media_finder::collect_data(config.directory_to_monitor)));

      // NOTE: kinda wrong approach (mb even completely??)
      if (std::chrono::steady_clock::now() >= end_time)
      {

        std::cout << "overrun; running next scan immediately\n";
        end_time = std::chrono::steady_clock::now();
      }
      else
      {
        std::this_thread::sleep_until(end_time);
      }
    }
  }
  catch (const CLI::ParseError& e)
  {
    arguments_handler.exit(e);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
}
