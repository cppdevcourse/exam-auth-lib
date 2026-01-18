#pragma once

#include <atomic>
#include <string_view>

#include "udp_session.hpp"
#include "udp_socket.hpp"

namespace auth
{
namespace server
{

class Server
{
public:
    Server(std::string_view adminUsername, std::string_view adminPassword,
        const int port, const std::atomic<bool>& running);
    void Run();
    void Stop();

private:
    void ProcessIncome();

private:
    const std::atomic<bool>& running_;
    UdpSocket socket_;
    UdpSession session_;
};

} // namespace server
} // namespace auth
