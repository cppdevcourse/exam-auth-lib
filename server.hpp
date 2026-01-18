#pragma once

#include <string_view>
namespace auth
{
namespace server
{

class Server
{
public:
    Server(std::string_view adminUsername, std::string_view adminPassword,
        const int port);
    void Run();

private:
    const int port_;
};

} // namespace server
} // namespace auth
