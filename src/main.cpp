#include <CLI/CLI.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <httplib.h>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <ostream>
#include <string>
#include <thread>
#include "config.hpp"
#include "handle_cli_options.hpp"
#include "json_message_data.hpp"
#include "log_error.hpp"
#include "media_scanner.hpp"

static void
launch_async_http_server(
  const std::string& host,
  size_t port,
  const std::shared_ptr< media_finder::json_message_data >& output)
{
  std::thread server_thread{
    [output, host, port]
    {
      httplib::Server server;

      server.Get("/",
                 [&output](const httplib::Request&, httplib::Response& res)
                 { res.set_content(output->get_data(), "application/json"); });

      server.listen(host, port);
    }
  };
  server_thread.detach();
}

int
main(int argc, char** argv)
{
  try
  {
    media_finder::config config;
    CLI::App arguments_handler{ "Small (media)file poller server",
                                "media_finder" };
    try
    {
      config = media_finder::handle_cli_options(argc, argv, arguments_handler);
    }
    catch (const CLI::ParseError& e)
    {
      arguments_handler.exit(e);
      return -1;
    }

    std::cout << "starting with directory = " << config.directory_to_monitor
              << ", interval = " << config.interval.count() << " seconds\n";

    auto output_message =
      std::make_shared< media_finder::json_message_data >("{}");

    media_finder::media_scanner scanner;

    std::string host = "127.0.0.1";
    size_t port = 1234;
    launch_async_http_server(host, port, output_message);

    auto end_time = std::chrono::steady_clock::now();
    for (;;)
    {
      end_time += config.interval;

      scanner.run_async_scan(output_message, config.directory_to_monitor);

      std::this_thread::sleep_until(end_time);
    }
  }
  catch (const std::exception& e)
  {
    media_finder::log_error(e.what());
    return -1;
  }
}
