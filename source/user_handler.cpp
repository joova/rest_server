#include "user_handler.hpp"
#include "user_manager.hpp"

using namespace web;
using namespace http;
using namespace utility;

void UserHandler::HandleCreate(const http_request & request) {
    request.extract_json()
    .then([=](json::value val){
        try {
            //BOOST_LOG_TRIVIAL(debug) << "val = " << val;
            UserInfo user(val);
            
            //BOOST_LOG_TRIVIAL(debug) << "userMgr.Create";
            UserManager().Create(user);
            
            json::value response;
            response[U("message")] = json::value::string(U("Insert success."));
            request.reply(status_codes::OK, response);
        }
        catch (DbException & e){
            //BOOST_LOG_TRIVIAL(error) << "Create user error: " << e.what();

            json::value error;
            string_t u_what = conversions::to_string_t(e.what());
            error[U("message")] = json::value::string(u_what);
            request.reply(status_codes::BadRequest, error);
        }
    });
}

void UserHandler::HandleUpdate(const http_request & request, const std::string & id) {
    request.extract_json()
    .then([=](json::value val){
        try {
            //BOOST_LOG_TRIVIAL(debug) << "val = " << val;
            UserInfo user(val);
            
            //BOOST_LOG_TRIVIAL(debug) << "userMgr.Update";
            UserManager().Update(id, user);
            
            json::value response;
            response[U("message")] = json::value::string(U("Update success."));
            request.reply(status_codes::OK, response);
        }
        catch (DbException & e){
            //BOOST_LOG_TRIVIAL(error) << "Update user error: " << e.what();
            json::value error;
            string_t u_what = conversions::to_string_t(e.what());
            error[U("message")] = json::value::string(u_what);
            request.reply(status_codes::BadRequest, error);
        }
    });
}

void UserHandler::HandleDelete(const http_request & request, const std::string & id) {
    try {
        UserManager().Delete(id);
        
        json::value response;
        response[U("message")] = json::value::string(U("Update success."));
        request.reply(status_codes::OK, response);
    }
    catch (const std::exception & e){
        json::value error;
        string_t u_what = conversions::to_string_t(e.what());
        error[U("message")] = json::value::string(u_what);
        request.reply(status_codes::BadRequest, error);
    }
}

void UserHandler::HandleList(const http_request & request) {
    try {
        std::vector<UserInfo> userList = UserManager().List();
        auto content = json::value::array();
        int index=0;
        for (UserInfo user : userList) {
            auto juser = user.ToJsonObject();
            content[index] = juser;
            index++;
        }

        http_response response(status_codes::OK);
        response.headers().add(U("Content-Type"), "application/json");
        response.headers().add(U("Pagination-Count"), userList.size());
        response.headers().add(U("Pagination-Page"), 1);
        response.headers().add(U("Pagination-Limit"), -1);
        response.set_body(content);

        request.reply(response);
    }
    catch (const std::exception & e) {
        json::value error;
        string_t u_what = conversions::to_string_t(e.what());
        error[U("message")] = json::value::string(u_what);
        request.reply(status_codes::InternalError, error);
    }
}

void UserHandler::HandleFind(const http_request & request, const int & offset, const int & limit) {
    UserManager userMgr;
    std::vector<UserInfo> userList = userMgr.Find(offset, limit);
    auto content = json::value::array();
    int index=0;
    for (UserInfo user : userList) {
        auto juser = user.ToJsonObject();
        content[index] = juser;
        index++;
    }
    int64_t count = userMgr.Count();

    http_response response(status_codes::OK);
    response.headers().add(U("Content-Type"), "application/json");
    response.headers().add(U("Pagination-Count"), count);
    response.headers().add(U("Pagination-Page"), count / limit);
    response.headers().add(U("Pagination-Limit"), limit);
    response.set_body(content);

    request.reply(response);
}

void UserHandler::HandleFindOne(const http_request & request, const std::string & id) {
    try {
        UserInfo user = UserManager().FindOne(id);
        auto response = user.ToJsonObject();\
        request.reply(status_codes::OK, response);
    }
    catch (const std::exception & e){
        json::value error;
        string_t u_what = conversions::to_string_t(e.what());
        error[U("message")] = json::value::string(u_what);
        request.reply(status_codes::BadRequest, error);
    }
}

void UserHandler::HandleFindText(const http_request & request, const int & offset, const int & limit, const std::string & text) {
    UserManager userMgr;
    std::vector<UserInfo> userList = userMgr.Find(offset, limit, text);
    auto content = json::value::array();
    int index=0;
    for (UserInfo user : userList) {
        auto juser = user.ToJsonObject();
        content[index] = juser;
        index++;
    }

    int64_t count = userMgr.Count(text);

    http_response response(status_codes::OK);
    response.headers().add(U("Content-Type"), "application/json");
    response.headers().add(U("Pagination-Count"), count);
    response.headers().add(U("Pagination-Page"), (count / limit));
    response.headers().add(U("Pagination-Limit"), limit);
    response.set_body(content);

    request.reply(response);
}

