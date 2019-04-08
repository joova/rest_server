
#include "microsvc_controller.hpp"

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
        if (path.size() == 2 && path[0] == U("service") && path[1] == U("health")) {
            HealthHandler().HandleStatus(request);
        }
        else if (path.size() == 2 && path[0] == U("auth") && path[1] == U("signon")) {
            AuthHandler().HandleSignOn(request);
        }
        else if (path.size() == 2 && path[0] == U("idm") && path[1] == U("users")) {
            AuthHandler().AuthMiddleware(request);
            UserHandler().HandleList(request);
        }
        else if (path.size() == 3 && path[0] == U("idm") && path[1] == U("user")) {
            AuthHandler().AuthMiddleware(request);
            std::string _id = conversions::to_utf8string(path[2]);
            UserHandler().HandleFindOne(request, _id);
        }
        else if (path.size() == 4 && path[0] == U("idm") && path[1] == U("users")) {
            AuthHandler().AuthMiddleware(request);
            //cover offset & limit from path
            int offset = std::stoi(path[2]);
            int limit = std::stoi(path[3]);
            UserHandler().HandleFind(request, offset, limit);
        }
        else if (path.size() == 5 && path[0] == U("idm") && path[1] == U("users")) {
            AuthHandler().AuthMiddleware(request);
            //cover offset & limit from path
            int offset = std::stoi(path[2]);
            int limit = std::stoi(path[3]);
            std::string text = conversions::to_utf8string(path[4]);

            UserHandler().HandleFindText(request, offset, limit, text);
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
            AuthHandler().AuthMiddleware(request);
            UserHandler().HandleCreate(request);
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
            AuthHandler().AuthMiddleware(request);
            std::string _id = conversions::to_utf8string(path[2]);
            UserHandler().HandleUpdate(request, _id);
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
        AuthHandler().AuthMiddleware(request);
        std::string _id = conversions::to_utf8string(path[2]);
        UserHandler().HandleDelete(request, _id);
    }
}

json::value MicroServiceController::respNotImpl(const http::method & method) {
    auto response = json::value::object();
    response[U("ServiceName")] = json::value::string(U("C++ MicroService"));
    response[U("http_method")] = json::value::string(method);
    return response;
}