#include <iostream>
#include <cpprest/http_msg.h>
#include "microsvc_controller.hpp"
#include "user_manager.hpp"

using namespace web;
using namespace http;

void MicroServiceController::initRestHandlers() {
    _listener.support(methods::GET, std::bind(&MicroServiceController::handleGet, this, std::placeholders::_1));
    _listener.support(methods::POST, std::bind(&MicroServiceController::handlePost, this, std::placeholders::_1));
    _listener.support(methods::PUT, std::bind(&MicroServiceController::handlePut, this, std::placeholders::_1));
    _listener.support(methods::DEL, std::bind(&MicroServiceController::handleDelete, this, std::placeholders::_1));
}

void MicroServiceController::handleGet(http_request request) {
    auto path = requestPath(request);
    if (!path.empty()) {
        if (path[0] == "service" && path[1] == "health") {
            auto response = json::value::object();
            response["version"] = json::value::string("0.0.1");
            response["status"] = json::value::string("ready!");
            request.reply(status_codes::OK, response);
        }
        else if (path[0] == "user" && path[1] == "list") {
            UserManger userMgr;
            std::vector<UserInfo> userList = userMgr.list();
            auto response = json::value::array();
            int index=0;
            for (UserInfo user : userList) {
                auto juser = json::value::object();
                juser["username"] = json::value(user.username);
                juser["firstName"] = json::value(user.firstName);
                juser["lastName"] = json::value(user.lastName);
                response[index] = juser;
                index++;
            }
            request.reply(status_codes::OK, response);
        }
    }
    else {
        request.reply(status_codes::NotFound);
    }
}

void MicroServiceController::handlePost(http_request request) {
    auto path = requestPath(request);
    if (!path.empty()) {
        if (path[0] == "user" && path[1] == "create") {
            request.extract_json()
            .then([=](json::value val){
                try {
                    UserInfo user;
                    user.username = val.at(U("username")).as_string();
                    user.password = val.at(U("password")).as_string();
                    user.firstName = val.at(U("firstName")).as_string();
                    user.lastName = val.at(U("lastName")).as_string();
                    
                    UserManger userMgr;
                    userMgr.create(user);
                    
                    json::value response;
                    response["message"] = json::value::string("Insert success.");
                    request.reply(status_codes::OK, response);
                }
                catch (json::json_exception & e){
                    request.reply(status_codes::BadRequest, e.what());
                }
            });
        }
    }
    else {
        request.reply(status_codes::NotFound);
    }
}

void MicroServiceController::handlePut(http_request request) {
    request.reply(status_codes::NotImplemented, respNotImpl(methods::PUT));
}

void MicroServiceController::handleDelete(http_request request) {
    request.reply(status_codes::NotImplemented, respNotImpl(methods::DEL));
}

json::value MicroServiceController::respNotImpl(const http::method & method) {
    auto response = json::value::object();
    response["ServiceName"] = json::value::string("C++ MicroService");
    response["http_method"] = json::value::string(method);
    return response;
}