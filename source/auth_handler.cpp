#include "auth_handler.hpp"
#include "user_manager.hpp"
#include "crypto_utils.hpp"

using namespace web;
using namespace http;
using namespace utility;

AuthHandler::AuthHandler() {}

AuthHandler::~AuthHandler() {}

void AuthHandler::HandleSignOn(const http_request & request) {
    pplx::create_task([=]() -> std::tuple<bool, UserInfo> {
        auto headers = request.headers();
        if (headers.find(U("Authorization")) == headers.end()) {
            BOOST_LOG_TRIVIAL(error) << "Authorization header error";
            throw std::exception();
        }
        auto authHeader = headers[U("Authorization")];
        auto credsPos = authHeader.find(U("Basic"));
        if (credsPos == std::string::npos) {
            BOOST_LOG_TRIVIAL(error) << "Authorization method error";
            throw std::exception();
        }
        auto base64 = authHeader.substr(credsPos + std::string("Basic").length() + 1);
        if (base64.empty()) {
            BOOST_LOG_TRIVIAL(error) << "Authorization credential error";
            throw std::exception();
        }
        auto bytes = conversions::from_base64(base64);
        std::string creds(bytes.begin(), bytes.end());
        auto colonPos = creds.find(":");
        if (colonPos == std::string::npos) {
            BOOST_LOG_TRIVIAL(error) << "Authorization credential username/password error";
            throw std::exception();
        }
        auto username = creds.substr(0, colonPos);
        auto password = creds.substr(colonPos + 1, creds.size()  - colonPos - 1);
        BOOST_LOG_TRIVIAL(debug) << "Authorization username = " << username;            
        BOOST_LOG_TRIVIAL(debug) << "Authorization password = " << password;
        UserInfo user;         
        if (SignOn(username, password, user)) {
            BOOST_LOG_TRIVIAL(debug) << "Authorization signon = true";
            return std::make_tuple(true, user);
        }
        else {
            BOOST_LOG_TRIVIAL(debug) << "Authorization signon = false";
            return std::make_tuple(false, UserInfo {});
        }
    })
    .then([=](pplx::task<std::tuple<bool, UserInfo>> resultTsk) {
        try {
            auto result = resultTsk.get();
            if (std::get<0>(result) == true) {
                std::string token = CreateToken();
                BOOST_LOG_TRIVIAL(debug) << "Authorization token = " << token;

                json::value response;
                response[U("token")] = json::value::string( conversions::to_string_t(token));                    
                request.reply(status_codes::OK, response);
            }
            else {
                BOOST_LOG_TRIVIAL(debug) << "Authorization result = false";
                request.reply(status_codes::Unauthorized);
            }
        }
        catch(const std::exception & e) {
            BOOST_LOG_TRIVIAL(error) << "Authorization error: " << e.what();
            request.reply(status_codes::Unauthorized);
        }
    });
}

void AuthHandler::AuthMiddleware(const http_request & request) {
    pplx::create_task([=]() -> std::tuple<bool, UserInfo> {
        auto headers = request.headers();
        if (headers.find(U("Authorization")) == headers.end()) {
            BOOST_LOG_TRIVIAL(error) << "Authorization header error";
            throw std::exception();
        }
        auto authHeader = headers[U("Authorization")];
        auto credsPos = authHeader.find(U("Bearer"));
        if (credsPos == std::string::npos) {
            BOOST_LOG_TRIVIAL(error) << "Authorization method error";
            throw std::exception();
        }
        auto wtoken = authHeader.substr(credsPos + std::string("Bearer").length() + 1);
        if (wtoken.empty()) {
            BOOST_LOG_TRIVIAL(error) << "Authorization token error";
            throw std::exception();
        }

        BOOST_LOG_TRIVIAL(debug) << "Authorization Bearer token = " << wtoken;

        UserInfo user{};
        std::string token;
        token.assign(wtoken.begin(), wtoken.end());         
        if (VerifyToken(token)) {
            BOOST_LOG_TRIVIAL(debug) << "Authorization token = true";
            return std::make_tuple(true, user);
        }
        else {
            BOOST_LOG_TRIVIAL(debug) << "Authorization token = false";
            return std::make_tuple(false, UserInfo {});
        }
    })
    .then([=](pplx::task<std::tuple<bool, UserInfo>> resultTsk) {
        try {
            auto result = resultTsk.get();
            if (std::get<0>(result) == true) {
                std::string token = CreateToken();
                BOOST_LOG_TRIVIAL(debug) << "Authorization token = " << token;
                return;
            }
            else {
                BOOST_LOG_TRIVIAL(debug) << "Authorization result = false";
                request.reply(status_codes::Unauthorized);
            }
        }
        catch(const std::exception & e) {
            BOOST_LOG_TRIVIAL(error) << "Authorization error: " << e.what();
            request.reply(status_codes::Unauthorized);
        }
    });
}

bool AuthHandler::SignOn(std::string username, std::string password, UserInfo & user) {
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

std::string AuthHandler::CreateToken(){
    auto token = jwt::create()
        .set_issuer("joova")
        .set_type("JWS")
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{60})
        .sign(jwt::algorithm::hs256{"topsecret"});
    return token;
}

bool AuthHandler::VerifyToken(std::string token) {
    auto verify = jwt::verify()
        .allow_algorithm(jwt::algorithm::hs256{"topsecret"})
        .with_issuer("joova");
    auto decoded_token = jwt::decode(token);
    try {
        verify.verify(decoded_token);
        return true;
    } catch (const jwt::token_verification_exception & e) {
        throw;
    }
    return false;
}