#include "json_message_data.hpp"

media_finder::json_message_data::json_message_data(const std::string& rhs) :
  mtx_(), data_(rhs)
{}

std::string
media_finder::json_message_data::get_data() const
{
  std::lock_guard l{ mtx_ };
  return data_;
}

void
media_finder::json_message_data::set_data(std::string rhs)
{
  std::lock_guard l{ mtx_ };
  data_ = std::move(rhs);
}
