#include "auth_manager.hpp"
#include "user_manager.hpp"
#include "crypto_utils.hpp"


bool AuthManager::SignOn(std::string username, std::string password, UserInfo & user) noexcept (false) {
    UserManager user_mgr;
    try {
        user = user_mgr.FindUsername(username);
        std::string password_hash = CryptoUtils::SHA256(password);
        if(user.password == password_hash) {
            return true;
        } else {
            return false;
        }
    } catch (const DbException & e) {
        throw DbException(std::string(e.what()));
    }
    
    return false;
}