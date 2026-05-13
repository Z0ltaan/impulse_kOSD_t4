#include <chrono>
#include <ctime>
#include <exception>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

namespace media_finder
{
  void print_help_message(std::ostream& out)
  {
    out << "usage: media_finder <directory_path> <timeout_in_seconds>\n";
  }
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
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
}
