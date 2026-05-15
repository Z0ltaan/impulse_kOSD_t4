#include "media_scanner.hpp"

#include <exception>
#include <thread>
#include "collect_data.hpp"
#include "log_error.hpp"

media_finder::media_scanner::media_scanner() :
  is_scanning_(std::make_shared< std::atomic< bool > >(false))
{}

void
media_finder::media_scanner::run_async_scan(
  std::shared_ptr< json_message_data > output,
  const std::filesystem::path& directory_to_monitor)
{
  bool expected = false;
  if (!is_scanning_->compare_exchange_strong(expected, true))
  {
    log_error("overrun; skipping this interval (if you see this message "
              "often then try bigger interval value)\n");
    return;
  }

  std::thread async_scan{
    [is_scanning_capture = this->is_scanning_, output, directory_to_monitor]
    {
      try
      {
        auto data = collect_data(directory_to_monitor);
        output->set_data(std::move(data));
      }
      catch (const std::exception& e)
      {
        log_error(e.what());
      }

      is_scanning_capture->store(false);
    }
  };

  async_scan.detach();
}
