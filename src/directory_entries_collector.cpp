#include "directory_entries_collector.hpp"

const std::map< std::string, std::string >
  media_finder::directory_entries_collector::extension_category_mapping = {
    { ".wav", "audio" },   { ".aac", "audio" },   { ".wma", "audio" },
    { ".flac", "audio" },  { ".ogg", "audio" },   { ".mp3", "audio" },
    { ".alac", "audio" },  { ".aiff", "audio" },  { ".mp4", "video" },
    { ".mov", "video" },   { ".mkv", "video" },   { ".webm", "video" },
    { ".wmv", "video" },   { ".avi", "video" },   { ".jpeg", "images" },
    { ".jpg", "images" },  { ".png", "images" },  { ".webp", "images" },
    { ".gif", "images" },  { ".avif", "images" }, { ".svg", "images" },
    { ".tiff", "images" }, { ".heif", "images" }, { ".heic", "images" },
  };

media_finder::directory_entries_collector::directory_entries_collector() :
  categorized_media_{}
{
  clear();
}

void
media_finder::directory_entries_collector::operator()(
  const std::filesystem::directory_entry& entry)
{
  if (!entry.is_regular_file())
  {
    return;
  }

  auto ext = extension_category_mapping.find(entry.path().extension().string());

  if (ext != extension_category_mapping.end())
  {
    categorized_media_[ext->second].push_back(entry.path().string());
  }
}

void
media_finder::directory_entries_collector::clear()
{
  categorized_media_ = { { "audio", json::array() },
                         { "video", json::array() },
                         { "images", json::array() } };
}

std::string
media_finder::directory_entries_collector::to_json_string() const
{
  return categorized_media_.dump();
}
