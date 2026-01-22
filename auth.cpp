#include "auth.hpp"

namespace auth
{
namespace core
{

    AuthManager::AuthManager() = default;

    void AuthManager::AddAdmin(std::string username, std::string password) 
    {
        users_.emplace(std::move(username), User{std::move(password), true});
    }

    std::optional<uint64_t> AuthManager::Login(const std::string& username, const std::string& password)
    {
        const auto it = users_.find(username);
        if (it == users_.end() ||
            it->second.password != password) 
                return std::nullopt;

        const uint64_t id = AcquireSessionId();

        sessions_[id] = username;
        return id;
    }

    bool AuthManager::Logout(uint64_t sessionId)
    {
        const auto erased = sessions_.erase(sessionId);
        if (erased == 0) return false;

        ReleaseSessionId(sessionId);
        return true;
    }

    bool AuthManager::AddUser(uint64_t sessionId, const std::string& username, const std::string& password)
    {
        if (!SessionIsAdmin(sessionId) || users_.count(username) != 0) return false;

        users_.emplace(username, User{password, false});
        return true;
    }

    bool AuthManager::DelUser(uint64_t sessionId, const std::string& username)
    {
        if (!SessionIsAdmin(sessionId)) return false;

        const auto erased = users_.erase(username);
        if (erased == 0) return false;

        for (auto it = sessions_.begin(); it != sessions_.end(); )
        {
            if (it->second == username)
            {
                ReleaseSessionId(it->first);
                it = sessions_.erase(it);
            }
            else
            {
                ++it;
            }
        }

        return true;
    }

    bool AuthManager::SessionExists(uint64_t sessionId)
    {
        return sessions_.count(sessionId) != 0;
    }

    bool AuthManager::SessionIsAdmin(uint64_t sessionId)
    {
        auto sit = sessions_.find(sessionId);
        if (sit == sessions_.end()) return false;

        auto uit = users_.find(sit->second);
        if (uit == users_.end()) return false;

        return uit->second.isAdmin;
    }

    uint64_t AuthManager::AcquireSessionId()
    {
        if (!freeSessions_.empty())
        {
            const uint64_t id = freeSessions_.top();
            freeSessions_.pop();
            return id;
        }

        return nextSessionId_++;
    }

    void AuthManager::ReleaseSessionId(uint64_t id)
    {
        freeSessions_.push(id);
    }
} // namespace core
} // namespace auth
