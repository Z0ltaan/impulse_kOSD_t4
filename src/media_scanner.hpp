#ifndef MEDIA_SCANNER_HPP
#define MEDIA_SCANNER_HPP

#include <atomic>
#include <filesystem>
#include <memory>
#include "json_message_data.hpp"

namespace media_finder
{
  class media_scanner
  {
  public:
    media_scanner();

    void run_async_scan(std::shared_ptr< json_message_data > output,
                        const std::filesystem::path& directory_to_monitor);

  private:
    std::shared_ptr< std::atomic< bool > > is_scanning_;
  };
}

#endif
