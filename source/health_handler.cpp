#include "health_handler.hpp"

using namespace web;
using namespace http;

void HealthHandler::HandleStatus(const http_request & request) {
    auto response = json::value::object();
    response[U("service")] = json::value::string(U("IDM API"));
    response[U("version")] = json::value::string(U("0.0.1"));
    response[U("status")] = json::value::string(U("ready!"));
    request.reply(status_codes::OK, response);
}