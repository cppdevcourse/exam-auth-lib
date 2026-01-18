#pragma once

#include <string_view>
#include <variant>

namespace auth
{
namespace server
{

namespace args
{

struct Login
{
    std::string_view username;
    std::string_view password;
};

struct Logout
{
    std::string_view sessionId;
};

struct Adduser
{
    std::string_view sessionId;
    std::string_view username;
    std::string_view password;
};

struct Deluser
{
    std::string_view sessionId;
    std::string_view username;
};

} // namespace args

class Command
{
public:
    using Args =
        std::variant<args::Login, args::Logout, args::Adduser, args::Deluser>;
    explicit Command(std::string_view str);
    const Args& GetArgs() const noexcept;

private:
    Args args_;
};

} // namespace server
} // namespace auth
