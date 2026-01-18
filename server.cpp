#include "server.hpp"
#include <iostream>

namespace auth
{
namespace server
{

Server::Server(std::string_view adminUsername,
    [[maybe_unused]] std::string_view adminPassword, const int port)
    : port_(port)
{
    std::cout << "Add administrator user '" << adminUsername << "/***'\n";
}

void Server::Run()
{
}

} // namespace server
} // namespace auth
