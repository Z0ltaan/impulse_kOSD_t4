#ifndef LOG_ERROR_HPP
#define LOG_ERROR_HPP

#include <mutex>
#include <string>

namespace media_finder
{

  inline std::mutex& get_log_mutex()
  {
    static std::mutex mtx;
    return mtx;
  }

  void log_error(const std::string& msg);
}

#endif
