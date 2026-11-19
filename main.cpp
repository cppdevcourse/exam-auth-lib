#include <atomic>
#include <charconv>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <system_error>

#include "server.hpp"

namespace
{

std::atomic<bool> server_running = true;

void print_usage(std::string_view basename)
{
    std::cout << "Usage: " << basename
              << " <ADMIN_USERNAME> <ADMIN_PASSWORD> <UDP_PORT>\n";
}

int parse_port(const char* const str)
{
    int port = 0;

    const auto* const begin = str;
    const auto* const end = str + std::strlen(str);
    const auto result = std::from_chars(begin, end, port);

    if (result.ec != std::errc{} || result.ptr != end || port <= 0 ||
        port > 65535)
    {
        throw std::out_of_range("Invalid port number");
    }

    return port;
}

} // namespace

int main(int argc, char* argv[])
{
    if (argc == 2 && std::string_view(argv[1]) == "-h")
    {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
    }

    if (argc < 4)
    {
        std::cerr << "Not enough arguments!\n";
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    try
    {
        const int port = parse_port(argv[3]);
        auth::server::Server server{argv[1], argv[2], port, server_running};

        auto stopServer = [](int)
        {
            std::cout << "Stopping server...\n";
            server_running.store(false, std::memory_order_relaxed);
        };
        std::signal(SIGINT, stopServer);
        std::signal(SIGTERM, stopServer);

        server.Run();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& err)
    {
        std::cerr << "Auth server failure: " << err.what() << '\n';
    }
    return EXIT_FAILURE;
}
