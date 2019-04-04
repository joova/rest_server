#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/oid.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include "user_manager.hpp"
#include "db_pool_singleton.hpp"
#include "crypto_utils.hpp"

using namespace utility;

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

UserInfo::UserInfo() {
    BOOST_LOG_TRIVIAL(debug) << "UserInfo::UserInfo()";
}

UserInfo::UserInfo(json::value val) {
    BOOST_LOG_TRIVIAL(debug) << "UserInfo::UserInfo(val) start";

    //TODO: belum bisa menangani jika key yg dikirim tidak lengkap
    if(val.has_field(U("username")))
        username = conversions::to_utf8string(val.at(U("username")).as_string());

    if(val.has_field(U("username")))
        password = conversions::to_utf8string(val.at(U("password")).as_string());

    if(val.has_field(U("first_name")))
        first_name = conversions::to_utf8string(val.at(U("first_name")).as_string());

    if(val.has_field(U("last_name")))
        last_name = conversions::to_utf8string(val.at(U("last_name")).as_string());
    
    BOOST_LOG_TRIVIAL(debug) << "UserInfo::UserInfo(val) end";
} 

json::value UserInfo::ToJsonObject() {
    BOOST_LOG_TRIVIAL(debug) << "UserInfo::ToJsonObject()";
    auto json_obj = json::value::object();

    if(!_id.empty()) {
        string_t u_id = conversions::to_string_t(this->_id);
        json_obj[U("_id")] = json::value(u_id);
    }

    string_t u_username = conversions::to_string_t(this->username);    
    json_obj[U("username")] = json::value(u_username);

    string_t u_first_name = conversions::to_string_t(this->first_name);
    json_obj[U("first_name")] = json::value(u_first_name);

    string_t u_last_name = conversions::to_string_t(this->last_name);
    json_obj[U("last_name")] = json::value(u_last_name);
    
    return json_obj;
}

UserManager::UserManager() {    
    BOOST_LOG_TRIVIAL(debug) << "UserManager::UserManager()";

    auto conn = DbPoolSingleton::instance().connection();
    BOOST_LOG_TRIVIAL(debug) << "connection().acquire()";

    auto db = (*conn)[DbPoolSingleton::instance().config().database()];
    
    BOOST_LOG_TRIVIAL(debug) << "db[user]";
    _collection = db["users"];
}

UserManager::~UserManager() { }

std::string UserManager::Create(const UserInfo & user) throw() {    
    auto builder = document{};
    bsoncxx::document::value doc_value = builder 
        << "username" << user.username
        << "password" << CryptoUtils::SHA256(user.password)
        << "first_name" << user.first_name
        << "last_name" << user.last_name
        << finalize;
    try {
        bsoncxx::stdx::optional<mongocxx::result::insert_one> 
        result = _collection.insert_one(std::move(doc_value));
        if (result) {
            return result->inserted_id().get_oid().value.to_string();
        } else {
            return "";
        }
    }
    catch (const std::exception& e) {
        throw DbException(std::string(e.what()));
    }
}

int UserManager::Update(const std::string _id, UserInfo & user) throw() {    
    auto builder = document{};
    bsoncxx::document::value doc_filter = builder 
            << "_id" << bsoncxx::oid{
                    bsoncxx::stdx::string_view{_id}
                }
            << finalize ;
    bsoncxx::document::value doc_value = builder 
        << "$set" << open_document
        << "first_name" << user.first_name
        << "last_name" << user.last_name
        << close_document
        << finalize;

    try {
        bsoncxx::stdx::optional<mongocxx::result::update> 
        result = _collection.update_one(std::move(doc_filter),std::move(doc_value));
        if (result) {
            return result->modified_count();
        } else {
            return 0;
        }
    }
    catch (const std::exception& e) {
        throw DbException(std::string(e.what()));
    }
}

// Delete - delete user by id
int UserManager::Delete(std::string _id) throw () {
    bsoncxx::stdx::optional<mongocxx::result::delete_result> 
        result = _collection.delete_one({
            document{} 
                << "_id" << bsoncxx::oid{
                        bsoncxx::stdx::string_view{_id}
                    }
            << finalize 
        });

    if(result) {
        int dcount = result->deleted_count();
        return dcount;
    } else {
        return 0;
    }
}

// List - ambil semua user
std::vector<UserInfo> UserManager::List() {
    std::vector<UserInfo> users;
    auto cursor = _collection.find({});
    for (auto doc : cursor) {
        UserInfo user = getUserInfo(doc);
        users.push_back(user);
    }
    return users;
}

UserInfo UserManager::FindOne(std::string _id) throw () {
    bsoncxx::stdx::optional<bsoncxx::document::value> 
        ovalue = _collection.find_one({
            document{} 
                << "_id" << bsoncxx::oid{
                        bsoncxx::stdx::string_view{_id}
                    }
            << finalize 
        });

    if(ovalue) {
        auto doc = ovalue->view();
        UserInfo user = getUserInfo(doc);
        return user;
    } else {
        throw DbException(std::string("User not found."));
    }
}

UserInfo UserManager::FindUsername(std::string username) throw () {
    bsoncxx::stdx::optional<bsoncxx::document::value> 
        ovalue = _collection.find_one({
            document{} 
                << "username" << username
            << finalize 
        });

    if(ovalue) {
        auto doc = ovalue->view();
        UserInfo user = getUserInfo(doc);
        return user;
    } else {
        throw DbException(std::string("User not found."));
    }
}

std::vector<UserInfo> UserManager::Find(int offset, int limit) {
    std::vector<UserInfo> users;
    mongocxx::options::find opts;
    opts.skip(offset);
    opts.limit(limit);
    
    auto cursor = _collection.find({}, opts);
    for (auto doc : cursor) {
        UserInfo user = getUserInfo(doc);
        users.push_back(user);
    }
    return users;
}

std::vector<UserInfo> UserManager::Find(int offset, int limit, std::string text) {
    std::vector<UserInfo> users;
    mongocxx::options::find opts;
    opts.skip(offset);
    opts.limit(limit);
    
    auto cursor = _collection.find({
        document{} << "$text" 
            << open_document 
            << "$search" <<  text 
            << close_document 
        << finalize
    }, opts);

    for (auto doc : cursor) {
        UserInfo user = getUserInfo(doc);
        users.push_back(user);
    }

    return users;
}

// Count - jumlah semua user
long UserManager::Count() {
    std::vector<UserInfo> users;
    long count  = _collection.count_documents({});
    return count;
}

// Count - jumlah data difilter berdasarkan text
long UserManager::Count(std::string text) {
    std::vector<UserInfo> users;
    long count  = _collection.count_documents({
        document{} << "$text" 
            << open_document 
            << "$search" <<  text 
            << close_document 
        << finalize
    });
    return count;
}

UserInfo UserManager::getUserInfo(const bsoncxx::document::view & doc){
    UserInfo user{};

    bsoncxx::document::element eid = doc["_id"];
    if(eid.type() == bsoncxx::type::k_oid) {
        user._id = eid.get_oid().value.to_string();
    }

    bsoncxx::document::element eusername = doc["username"];
    user.username = eusername.get_utf8().value.to_string();
    user.password = doc["password"].get_utf8().value.to_string();
    user.first_name = doc["first_name"].get_utf8().value.to_string();
    user.last_name = doc["last_name"].get_utf8().value.to_string();
    
    return user;
}

