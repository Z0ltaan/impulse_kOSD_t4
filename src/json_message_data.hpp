#ifndef JSON_MESSAGE_DATA_HPP
#define JSON_MESSAGE_DATA_HPP

#include <mutex>
#include <string>

namespace media_finder
{
  class json_message_data
  {
  public:
    explicit json_message_data(const std::string& rhs);

    std::string get_data() const;

    void set_data(std::string rhs);

  private:
    mutable std::mutex mtx_;
    std::string data_;
  };

}

#endif
