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
    void Create(const UserInfo & user) throw ();
    void Update(const std::string _id, UserInfo & user) throw ();
    UserInfo FindOne(std::string _id) throw ();
    std::vector<UserInfo> Find(int offset, int limit);
    std::vector<UserInfo> Find(int offset, int limit, std::string text);
    
private:
    mongocxx::collection _collection;
    UserInfo getUserInfo(const bsoncxx::document::view & doc);
};