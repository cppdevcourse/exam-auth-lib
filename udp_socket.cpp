#include <stdexcept>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "udp_socket.hpp"

namespace auth
{
namespace server
{

UdpSocket::UdpSocket(const int port)
{
    fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd_ < 0)
    {
        throw std::runtime_error("failed to open UDP socket");
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(fd_, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        close(fd_);
        throw std::runtime_error("failed to bind socket to address");
    }
}

int UdpSocket::Handler() const noexcept
{
    return fd_;
}

UdpSocket::~UdpSocket() noexcept
{
    close(fd_);
}

} // namespace server
} // namespace auth
