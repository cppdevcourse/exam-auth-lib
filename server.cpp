#include <cerrno>
#include <iostream>

#include <poll.h>
#include <stdexcept>

#include "server.hpp"
#include "udp_session.hpp"
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
    UdpSession session{socket};
    std::cout << "Listening on port " << port_ << '\n';

    pollfd handlers[1] = {{socket.Handler(), POLLIN, 0}};

    while (running_.load())
    {
        const int timeoutMs = 100;
        const int pollResult = poll(handlers, 1, timeoutMs);

        if (pollResult < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            throw std::runtime_error("failed to poll");
        }
        else if (pollResult == 0)
        {
            continue;
        }
        else if (handlers[0].revents & POLLIN)
        {
            const auto income{session.Receive()};
        }
    }
}

} // namespace server
} // namespace auth
