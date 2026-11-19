#include <cstddef>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "command.hpp"

namespace auth
{
namespace server
{

namespace
{

using Lexemes = std::vector<std::string_view>;

Lexemes split(std::string_view str)
{
    Lexemes lexemes{};
    std::size_t start = 0;

    while (start < str.size())
    {
        const std::size_t pos = str.find_first_of(" \t", start);
        if (pos == std::string_view::npos)
        {
            lexemes.emplace_back(str.substr(start));
            break;
        }

        if (pos > start)
        {
            lexemes.emplace_back(str.substr(start, pos - start));
        }

        start = pos + 1;
    }

    return lexemes;
}

void assert_arguments_number(const Lexemes& lexemes,
    const std::size_t expectedArgsNumber)
{
    if (lexemes.size() != expectedArgsNumber)
    {
        throw std::runtime_error("invalid arguments number");
    }
}

} // namespace

Command::Command(std::string_view str)
{
    const auto lexemes = split(str);
    if (lexemes.size() < 1)
    {
        throw std::runtime_error("empty command");
    }
    const auto action = lexemes[0];
    if (action == "login")
    {
        assert_arguments_number(lexemes, 3);
        args_ = args::Login{lexemes[1], lexemes[2]};
    }
    else if (action == "logout")
    {
        assert_arguments_number(lexemes, 2);
        args_ = args::Logout{lexemes[1]};
    }
    else if (action == "adduser")
    {
        assert_arguments_number(lexemes, 4);
        args_ = args::Adduser{lexemes[1], lexemes[2], lexemes[3]};
    }
    else if (action == "deluser")
    {
        assert_arguments_number(lexemes, 3);
        args_ = args::Deluser{lexemes[1], lexemes[2]};
    }
    else
    {
        throw std::runtime_error("unknown command");
    }
}

const Command::Args& Command::GetArgs() const noexcept
{
    return args_;
}

} // namespace server
} // namespace auth
