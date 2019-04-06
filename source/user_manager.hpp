#ifndef REST_SERVER_USER_MANAGER_H_
#define REST_SERVER_USER_MANAGER_H_


#include <string>
#include <vector>
#include <iostream>
#include <cpprest/http_msg.h>
#include <mongocxx/client.hpp>
#include <bsoncxx/json.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include "db_exception.hpp"

using namespace web;

class UserInfo {
public:
    std::string _id;
    std::string username;
    std::string password;
    std::string first_name;
    std::string last_name;

    UserInfo();
    UserInfo(json::value val);
    json::value ToJsonObject();

};

class UserManager {
public:
    UserManager();
    ~UserManager();

    std::vector<UserInfo> List();
    
    std::string Create(const UserInfo & user) noexcept(false);
    int Update(const std::string _id, UserInfo & user) noexcept(false);
    int Delete(std::string _id) noexcept(false);

    UserInfo FindOne(std::string _id) noexcept(false);
    UserInfo FindUsername(std::string username) noexcept(false);
    std::vector<UserInfo> Find(int offset, int limit);
    std::vector<UserInfo> Find(int offset, int limit, std::string text);
    
    int64_t Count();
    int64_t Count(std::string text);
    
    
private:
    mongocxx::collection _collection;
    UserInfo getUserInfo(const bsoncxx::document::view & doc);
};

#endif // REST_SERVER_USER_MANAGER_H_