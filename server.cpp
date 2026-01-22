#include <cerrno>
#include <cstddef>
#include <exception>
#include <iostream>

#include <poll.h>
#include <stdexcept>
#include <string_view>
#include <variant>

#include "server.hpp"
#include "udp_session.hpp"
#include "udp_socket.hpp"
#include "command.hpp"
#include "auth.hpp"

namespace auth
{
namespace server
{

static auth::core::AuthManager authManager;


namespace
{

std::string_view bytes_to_string(const UdpSession::Bytes& bytes)
{
    std::string_view str{reinterpret_cast<const char*>(bytes.data()),
        bytes.size()};
    str = str.substr(0, str.find_last_not_of(" \t\n\r\f\v") + 1);
    return str;
}

UdpSession::Bytes string_to_bytes(std::string_view str)
{
    UdpSession::Bytes bytes{};
    bytes.reserve(str.size());
    for (const auto c : str)
    {
        const auto byte{static_cast<std::byte>(static_cast<unsigned char>(c))};
        bytes.emplace_back(byte);
    }
    return bytes;
}

std::string process_command(std::string_view income)
{
    std::string answer{"not implemented\n"};
    try
    {
        Command command{income};
        const auto vargs = command.GetArgs();
        if (std::holds_alternative<args::Login>(vargs))
        {
            const auto args = std::get<args::Login>(vargs);
            const auto session = authManager.Login(std::string(args.username), std::string(args.password));

            if (!session)
                return "error: invalud username or password\n";

            return "session_id=" + std::to_string(*session) + "\n";
        }
        else if (std::holds_alternative<args::Logout>(vargs))
        {
            const auto args = std::get<args::Logout>(vargs);

            if (!authManager.Logout(std::stoull(std::string(args.sessionId)))) 
                return "error: invalid session\n";

            return "ok\n";
        }
        else if (std::holds_alternative<args::Adduser>(vargs))
        {
            const auto args = std::get<args::Adduser>(vargs);

            if (!authManager.AddUser(std::stoull(std::string(args.sessionId)),
                                     std::string(args.username),
                                     std::string(args.password)))
                return "error: access denied or user exists\n";
            
            return "ok\n";
        }
        else if (std::holds_alternative<args::Deluser>(vargs))
        {
            const auto args = std::get<args::Deluser>(vargs);

            if (!authManager.DelUser(std::stoull(std::string(args.sessionId)),
                                     std::string(args.username)))
                return "error: access denied or user not found\n";
            
            return "ok\n";
        }
    }
    catch (const std::exception& err)
    {
        answer = std::string("error: ") + err.what() + '\n';
    }
    return answer;
}

} // namespace

Server::Server(std::string_view adminUsername,
    [[maybe_unused]] std::string_view adminPassword, const int port,
    const std::atomic<bool>& running)
    : running_(running), socket_(port), session_(socket_)
{
    authManager.AddAdmin(std::string(adminUsername), std::string(adminPassword));
    
    std::cout << "Add administrator user '" << adminUsername << "/***'\n";
    std::cout << "Listening on port " << port << '\n';
}

void Server::Run()
{
    pollfd handlers[1] = {{socket_.Handler(), POLLIN, 0}};

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
            ProcessIncome();
        }
    }
}

void Server::ProcessIncome()
{
    try
    {
        const auto incomeRaw{session_.Receive()};
        const auto income{bytes_to_string(incomeRaw)};
        const auto outcome{process_command(income)};
        auto outcomeRaw{string_to_bytes(outcome)};
        session_.Reply(outcomeRaw);
    }
    catch (const std::exception& err)
    {
        std::cerr << "Error while processing income: " << err.what() << '\n';
    }
}

} // namespace server
} // namespace auth
