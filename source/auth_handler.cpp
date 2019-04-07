#include "auth_handler.hpp"
#include "user_manager.hpp"
#include "crypto_utils.hpp"

using namespace web;
using namespace http;
using namespace utility;

void AuthHandler::HandleSignOn(const http_request & request) {
    pplx::create_task([=]() -> std::tuple<bool, UserInfo> {
        auto headers = request.headers();
        if (request.headers().find(U("Authorization")) == headers.end()) 
            throw std::exception();
        auto authHeader = headers[U("Authorization")];
        auto credsPos = authHeader.find(U("Basic"));
        if (credsPos == std::string::npos) 
            throw std::exception();
        
        auto base64 = authHeader.substr(credsPos + std::string("Basic").length() + 1);
        if (base64.empty()) 
            throw std::exception();

        auto bytes = conversions::from_base64(base64);
        std::string creds(bytes.begin(), bytes.end());
        auto colonPos = creds.find(":");
        if (colonPos == std::string::npos) 
            throw std::exception();

        auto username = creds.substr(0, colonPos);
        auto password = creds.substr(colonPos + 1, creds.size()  - colonPos - 1);            

        UserInfo user;         
        if (SignOn(username, password, user)) {
            return std::make_tuple(true, user);
        }
        else {
            return std::make_tuple(false, UserInfo {});
        }
    })
    .then([=](pplx::task<std::tuple<bool, UserInfo>> resultTsk) {
        try {
            auto result = resultTsk.get();
            if (std::get<0>(result) == true) {
                json::value response;
                // std::stringstream ss;
                // ss << "welcome " << std::get<1>(result).first_name 
                //     << " " << std::get<1>(result).last_name << "!";
                response[U("success")] = json::value::string( conversions::to_string_t("Welcome"));                    
                request.reply(status_codes::OK, response);
            }
            else {
                request.reply(status_codes::Unauthorized);
            }
        }
        catch(std::exception) {
            request.reply(status_codes::Unauthorized);
        }
    });
}

bool AuthHandler::SignOn(std::string username, std::string password, UserInfo & user) noexcept (false) {
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