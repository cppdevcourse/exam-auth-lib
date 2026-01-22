#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <optional>
#include <stack>

namespace auth
{
namespace core
{
    struct User
    {
        std::string password;
        bool isAdmin = false;
    };

    class AuthManager
    {
    public:
        AuthManager();

        void AddAdmin(std::string username, std::string password);

        std::optional<uint64_t> Login(const std::string& username, const std::string& password);

        bool Logout(uint64_t sessionId);

        bool AddUser(uint64_t sessionId, const std::string& username, const std::string& password);

        bool DelUser(uint64_t sessionId, const std::string& username);

        bool SessionExists(uint64_t sessionId);
        bool SessionIsAdmin(uint64_t sessionId);
    private:
        uint64_t AcquireSessionId();
        void ReleaseSessionId(uint64_t id);

        std::unordered_map<std::string, User> users_;
        std::unordered_map<uint64_t, std::string> sessions_;

        std::stack<uint64_t> freeSessions_;
        uint64_t nextSessionId_ = 1;
    };
} // namespace core
} // namespace auth
