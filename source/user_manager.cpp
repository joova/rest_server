#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include "user_manager.hpp"
#include "db_pool_singleton.hpp"

void UserManger::create(const UserInfo & user) {
    auto conn = DbPoolSingleton::shared().connection().acquire();
    auto db = (*conn)[DbPoolSingleton::_config.database()];

    auto coll = db["users"];

    bsoncxx::builder::stream::document document{};
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder 
                        << "username" << user.username
                        << "password" << user.password
                        << "firstName" << user.firstName
                        << "lastName" << user.lastName
                        << bsoncxx::builder::stream::finalize;
    coll.insert_one(std::move(doc_value));
}

std::vector<UserInfo> UserManger::list() {
    auto conn = DbPoolSingleton::shared().connection().acquire();
    auto db = (*conn)[DbPoolSingleton::_config.database()];
    
    std::vector<UserInfo> users;
    if ( db.has_collection("users") ) {
        auto cursor = db["users"].find({});
        for (auto doc : cursor) {
            UserInfo user;
            user.username = doc["username"].get_utf8().value.to_string();
            user.password = doc["password"].get_utf8().value.to_string();
            user.firstName = doc["firstName"].get_utf8().value.to_string();
            user.lastName = doc["lastName"].get_utf8().value.to_string();
            users.push_back(user);
        }
    }
    return users;
}