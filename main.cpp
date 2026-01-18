#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string_view>

#include "server.hpp"

namespace
{

void print_usage(std::string_view basename)
{
    std::cout << "Usage: " << basename
              << " <ADMIN_USERNAME> <ADMIN_PASSWORD> <UDP_PORT>\n";
}

int parse_port(const char* const str)
{
    const int port = std::stoi(str);
    static const int maxPort = 65535;
    if (port < 0 || port > maxPort)
    {
        throw std::out_of_range("Invalid port number");
    }
    return port;
}

} // namespace

int main(int argc, char* argv[])
{
    if (argc == 0)
    {
        std::cerr << "argc is zero\n";
        return EXIT_FAILURE;
    }

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
        auth::server::Server server{argv[1], argv[2], port};
        server.Run();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& err)
    {
        std::cerr << "Auth server failure: " << err.what() << '\n';
    }
    return EXIT_FAILURE;
}
