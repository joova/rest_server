#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>


#include "user_manager.hpp"
#include "db_pool_singleton.hpp"


using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

void UserManger::create(const UserInfo & user) throw(DbException) {
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
    try {
        coll.insert_one(std::move(doc_value));
    }
    catch (const std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Create user error: " << e.what();
        throw DbException(std::string(e.what()));
    }
    
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

UserInfo UserManger::find_one(std::string username) throw (DbException) {
    auto conn = DbPoolSingleton::shared().connection().acquire();
    auto db = (*conn)[DbPoolSingleton::_config.database()];
    
    if ( db.has_collection("users") ) {
        bsoncxx::stdx::optional<bsoncxx::document::value> 
            ovalue = db["users"].find_one({
                document{} 
                    << "username" << username
                << finalize 
            });

        if(ovalue) {
            UserInfo user;
            auto result = ovalue->view();
            
            user.username = result["username"].get_utf8().value.to_string();
            user.password = result["password"].get_utf8().value.to_string();
            user.firstName = result["firstName"].get_utf8().value.to_string();
            user.lastName = result["lastName"].get_utf8().value.to_string();
            
            return user;
        } else {
            throw DbException(std::string("User not found."));
        }
    } 
    else {
        throw DbException(std::string("Collection 'users' not found."));
    }
}

std::vector<UserInfo> UserManger::find(int offset, int limit) {
    auto conn = DbPoolSingleton::shared().connection().acquire();
    auto db = (*conn)[DbPoolSingleton::_config.database()];
    
    std::vector<UserInfo> users;
    if ( db.has_collection("users") ) {
        mongocxx::options::find opts;
        opts.skip(offset);
        opts.limit(limit);
        
        auto cursor = db["users"].find({}, opts);
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

std::vector<UserInfo> UserManger::find(int offset, int limit, std::string text) {
    auto conn = DbPoolSingleton::shared().connection().acquire();
    auto db = (*conn)[DbPoolSingleton::_config.database()];
    
    std::vector<UserInfo> users;
    if ( db.has_collection("users") ) {
        mongocxx::options::find opts;
        opts.skip(offset);
        opts.limit(limit);
        
        auto cursor = db["users"].find({
            document{} << "$text" 
                << open_document 
                << "$search" <<  text 
                << close_document 
            << finalize
        }, opts);

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