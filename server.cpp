#include <iostream>

#include "server.hpp"
#include "udp_socket.hpp"

namespace auth
{
namespace server
{

Server::Server(std::string_view adminUsername,
    [[maybe_unused]] std::string_view adminPassword, const int port,
    const std::atomic<bool>& running)
    : port_(port), running_(running)
{
    std::cout << "Add administrator user '" << adminUsername << "/***'\n";
}

void Server::Run()
{
    UdpSocket socket{port_};
    std::cout << "Listening on port " << port_ << '\n';
    while (running_)
    {
    }
}

} // namespace server
} // namespace auth
