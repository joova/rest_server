#include <iostream>
#include <cpprest/http_msg.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include "microsvc_controller.hpp"
#include "user_manager.hpp"

using namespace web;
using namespace http;
using namespace utility;

void MicroServiceController::InitRestHandlers() {
    _listener.support(methods::GET, std::bind(&MicroServiceController::HandleGet, this, std::placeholders::_1));
    _listener.support(methods::POST, std::bind(&MicroServiceController::HandlePost, this, std::placeholders::_1));
    _listener.support(methods::PUT, std::bind(&MicroServiceController::HandlePut, this, std::placeholders::_1));
    _listener.support(methods::DEL, std::bind(&MicroServiceController::HandleDelete, this, std::placeholders::_1));
}

void MicroServiceController::HandleGet(http_request request) {
    auto path = RequestPath(request);
    if (!path.empty()) {
        if (path.size() == 2 && path[0] == U("idm") && path[1] == U("health")) {
            auto response = json::value::object();
            response[U("service")] = json::value::string(U("IDM API"));
            response[U("version")] = json::value::string(U("0.0.1"));
            response[U("status")] = json::value::string(U("ready!"));
            request.reply(status_codes::OK, response);
        }
        else if (path.size() == 2 && path[0] == U("idm") && path[1] == U("users")) {
            try {
                UserManager userMgr;
                std::vector<UserInfo> userList = userMgr.List();
                auto response = json::value::array();
                int index=0;
                for (UserInfo user : userList) {
                    auto juser = user.ToJsonObject();
                    response[index] = juser;
                    index++;
                }
                request.reply(status_codes::OK, response);
            }
            catch (const std::exception & e) {
                json::value error;
                error[U("message")] = json::value::string(e.what());
                request.reply(status_codes::InternalError, error);
            }
        }
        else if (path.size() == 3 && path[0] == U("idm") && path[1] == U("user")) {
            std::string _id = path[2];

            try {
                UserManager userMgr;
                UserInfo user = userMgr.FindOne(_id);
                
                auto response = user.ToJsonObject();
            
                request.reply(status_codes::OK, response);
            }
            catch (const std::exception & e){
                json::value error;
                error[U("message")] = json::value::string(e.what());
                request.reply(status_codes::BadRequest, error);
            }
            
        }
        else if (path.size() == 4 && path[0] == U("idm") && path[1] == U("users")) {
            //cover offset & limit from path
            int offset = std::stoi(path[2]);
            int limit = std::stoi(path[3]);

            UserManager userMgr;
            std::vector<UserInfo> userList = userMgr.Find(offset, limit);
            auto response = json::value::array();
            int index=0;
            for (UserInfo user : userList) {
                auto juser = user.ToJsonObject();
                response[index] = juser;
                index++;
            }
            request.reply(status_codes::OK, response);
        }
        else if (path.size() == 5 && path[0] == U("idm") && path[1] == U("users")) {
            //cover offset & limit from path
            int offset = std::stoi(path[2]);
            int limit = std::stoi(path[3]);
            std::string text = path[4];

            UserManager userMgr;
            std::vector<UserInfo> userList = userMgr.Find(offset, limit, text);
            auto response = json::value::array();
            int index=0;
            for (UserInfo user : userList) {
                auto juser = user.ToJsonObject();
                response[index] = juser;
                index++;
            }
            request.reply(status_codes::OK, response);
        }
        else {
            request.reply(status_codes::NotFound);
        }
    }
    else {
        request.reply(status_codes::NotFound);
    }
}

void MicroServiceController::HandlePost(http_request request) {
    auto path = RequestPath(request);
    if (!path.empty()) {
        if (path[0] == U("idm") && path[1] == U("user")) {
            request.extract_json()
            .then([=](json::value val){
                try {
                    BOOST_LOG_TRIVIAL(debug) << "val = " << val;
                    UserInfo user(val);
                    
                    UserManager userMgr;
                    BOOST_LOG_TRIVIAL(debug) << "userMgr.Create";
                    userMgr.Create(user);
                    
                    json::value response;
                    response[U("message")] = json::value::string("Insert success.");
                    request.reply(status_codes::OK, response);
                }
                catch (DbException & e){
                    BOOST_LOG_TRIVIAL(error) << "Create user error: " << e.what();

                    json::value error;
                    error[U("message")] = json::value::string(e.what());
                    request.reply(status_codes::BadRequest, error);
                }
            });
        }
        else {
            request.reply(status_codes::NotFound);
        }
    }
    else {
        request.reply(status_codes::NotFound);
    }
}

void MicroServiceController::HandlePut(http_request request) {
    auto path = RequestPath(request);
    if (!path.empty()) {
        if (path.size() == 3 && path[0] == U("idm") && path[1] == U("user")) {
            request.extract_json()
            .then([=](json::value val){
                try {
                    BOOST_LOG_TRIVIAL(debug) << "val = " << val;
                    UserInfo user(val);
                    
                    std::string _id = path[2];

                    UserManager userMgr;
                    BOOST_LOG_TRIVIAL(debug) << "userMgr.Update";
                    userMgr.Update(_id, user);
                    
                    json::value response;
                    response[U("message")] = json::value::string("Update success.");
                    request.reply(status_codes::OK, response);
                }
                catch (DbException & e){
                    BOOST_LOG_TRIVIAL(error) << "Update user error: " << e.what();

                    json::value error;
                    error[U("message")] = json::value::string(e.what());
                    request.reply(status_codes::BadRequest, error);
                }
            });
        }
        else {
            request.reply(status_codes::NotFound);
        }
    }
    else {
        request.reply(status_codes::NotFound);
    }
}

void MicroServiceController::HandleDelete(http_request request) {
    request.reply(status_codes::NotImplemented, respNotImpl(methods::DEL));
}

json::value MicroServiceController::respNotImpl(const http::method & method) {
    auto response = json::value::object();
    response[U("ServiceName")] = json::value::string(U("C++ MicroService"));
    response[U("http_method")] = json::value::string(U(method));
    return response;
}