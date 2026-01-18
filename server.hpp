#pragma once

#include <atomic>
#include <string_view>
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
    const int port_;
    const std::atomic<bool>& running_;
};

} // namespace server
} // namespace auth
