#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <cpprest/http_msg.h>

#include "microsvc_controller.hpp"
#include "user_manager.hpp"
#include "auth_manager.hpp"

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
        else if (path.size() == 2 && path[0] == U("auth") && path[1] == U("signon")) {
            pplx::create_task([=]() -> std::tuple<bool, UserInfo> {
                auto headers = request.headers();
                if (request.headers().find("Authorization") == headers.end()) 
                    throw std::exception();
                auto authHeader = headers["Authorization"];
                auto credsPos = authHeader.find("Basic");
                if (credsPos == std::string::npos) 
                    throw std::exception();
                
                auto base64 = authHeader.substr(credsPos + std::string("Basic").length() + 1);
                if (base64.empty()) 
                    throw std::exception();

                auto bytes = utility::conversions::from_base64(base64);
                std::string creds(bytes.begin(), bytes.end());
                auto colonPos = creds.find(":");
                if (colonPos == std::string::npos) 
                    throw std::exception();

                auto username = creds.substr(0, colonPos);
                auto password = creds.substr(colonPos + 1, creds.size()  - colonPos - 1);            

                UserInfo user;
                AuthManager auth_mgr;         
                if (auth_mgr.SignOn(username, password, user)) {
                    return std::make_tuple(true, user);
                }
                else {
                return std::make_tuple(false, UserInfo {});
                }
            })
            .then([=](pplx::task<std::tuple<bool, UserInfo>> resultTsk) {
                try {
                    auto result = resultTsk.get();
                    if (std::get<0>(result) == true) {
                        json::value response;
                        response["success"] = json::value::string("welcome " + std::get<1>(result).first_name + " " 
                                    + std::get<1>(result).last_name + "!");                    
                        request.reply(status_codes::OK, response);
                    }
                    else {
                        request.reply(status_codes::Unauthorized);
                    }
                }
                catch(std::exception) {
                    request.reply(status_codes::Unauthorized);
                }
            });
        }
        else if (path.size() == 2 && path[0] == U("idm") && path[1] == U("users")) {
            try {
                UserManager userMgr;
                std::vector<UserInfo> userList = userMgr.List();
                auto content = json::value::array();
                int index=0;
                for (UserInfo user : userList) {
                    auto juser = user.ToJsonObject();
                    content[index] = juser;
                    index++;
                }

                http_response response(status_codes::OK);
                response.headers().add(U("Content-Type"), U("application/json"));
                response.headers().add(U("Pagination-Count"), U(userList.size()));
                response.headers().add(U("Pagination-Page"), U("1"));
                response.headers().add(U("Pagination-Limit"), U("-1"));
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
        else if (path.size() == 3 && path[0] == U("idm") && path[1] == U("user")) {
            std::string _id = conversions::to_utf8string(path[2]);

            try {
                UserManager userMgr;
                UserInfo user = userMgr.FindOne(_id);
                
                auto response = user.ToJsonObject();
            
                request.reply(status_codes::OK, response);
            }
            catch (const std::exception & e){
                json::value error;
                string_t u_what = conversions::to_string_t(e.what());
                error[U("message")] = json::value::string(u_what);
                request.reply(status_codes::BadRequest, error);
            }
            
        }
        else if (path.size() == 4 && path[0] == U("idm") && path[1] == U("users")) {
            //cover offset & limit from path
            int offset = std::stoi(path[2]);
            int limit = std::stoi(path[3]);

            UserManager userMgr;
            std::vector<UserInfo> userList = userMgr.Find(offset, limit);
            auto content = json::value::array();
            int index=0;
            for (UserInfo user : userList) {
                auto juser = user.ToJsonObject();
                content[index] = juser;
                index++;
            }
            long count = userMgr.Count();

            http_response response(status_codes::OK);
            response.headers().add(U("Content-Type"), U("application/json"));
            response.headers().add(U("Pagination-Count"), U(count));
            response.headers().add(U("Pagination-Page"), U(count / limit));
            response.headers().add(U("Pagination-Limit"), U(limit));
            response.set_body(content);

            request.reply(response);
        }
        else if (path.size() == 5 && path[0] == U("idm") && path[1] == U("users")) {
            //cover offset & limit from path
            int offset = std::stoi(path[2]);
            int limit = std::stoi(path[3]);
            std::string text = conversions::to_utf8string(path[4]);

            UserManager userMgr;
            std::vector<UserInfo> userList = userMgr.Find(offset, limit, text);
            auto content = json::value::array();
            int index=0;
            for (UserInfo user : userList) {
                auto juser = user.ToJsonObject();
                content[index] = juser;
                index++;
            }

            long count = userMgr.Count(text);

            http_response response(status_codes::OK);
            response.headers().add(U("Content-Type"), U("application/json"));
            response.headers().add(U("Pagination-Count"), U(count));
            response.headers().add(U("Pagination-Page"), U(count / limit));
            response.headers().add(U("Pagination-Limit"), U(limit));
            response.set_body(content);

            request.reply(response);
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
                    //BOOST_LOG_TRIVIAL(debug) << "val = " << val;
                    UserInfo user(val);
                    
                    UserManager userMgr;
                    //BOOST_LOG_TRIVIAL(debug) << "userMgr.Create";
                    userMgr.Create(user);
                    
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
                    //BOOST_LOG_TRIVIAL(debug) << "val = " << val;
                    UserInfo user(val);
                    
                    std::string _id = conversions::to_utf8string(path[2]);

                    UserManager userMgr;
                    //BOOST_LOG_TRIVIAL(debug) << "userMgr.Update";
                    userMgr.Update(_id, user);
                    
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
        else {
            request.reply(status_codes::NotFound);
        }
    }
    else {
        request.reply(status_codes::NotFound);
    }
}

void MicroServiceController::HandleDelete(http_request request) {
    auto path = RequestPath(request);
    if (path.size() == 3 && path[0] == U("idm") && path[1] == U("user")) {
        std::string _id = conversions::to_utf8string(path[2]);

        try {
            UserManager userMgr;
            userMgr.Delete(_id);
            
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
}

json::value MicroServiceController::respNotImpl(const http::method & method) {
    auto response = json::value::object();
    response[U("ServiceName")] = json::value::string(U("C++ MicroService"));
    response[U("http_method")] = json::value::string(method);
    return response;
}