#ifndef REST_SERVER_AUTH_MANAGER_H_
#define REST_SERVER_AUTH_MANAGER_H_


#include <string>
#include <vector>
#include <iostream>
#include <bsoncxx/json.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <cpprest/http_msg.h>
#include <mongocxx/client.hpp>
#include "db_pool_singleton.hpp"
#include "db_exception.hpp"
#include "user_manager.hpp"

using namespace web;

class AuthManager {
public:
    static bool SignOn(std::string username, std::string password, UserInfo & user) noexcept (false);
};

#endif // REST_SERVER_AUTH_MANAGER_H_