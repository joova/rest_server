#ifndef REST_SERVER_USER_HANDLER_H_
#define REST_SERVER_USER_HANDLER_H_

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <pplx/pplxtasks.h>
#include <cpprest/http_msg.h>

using namespace web;
using namespace http;


class UserHandler {
public:
    void HandleCreate(const http_request & request);
    void HandleUpdate(const http_request & request, const std::string & id);
    void HandleDelete(const http_request & request, const std::string & id);
    void HandleList(const http_request & request);
    void HandleFind(const http_request & request, const int & offset, const int & limit);
    void HandleFindOne(const http_request & request, const std::string & id);
    void HandleFindText(const http_request & request, const int & offset, const int & limit, const std::string & text);
};

#endif // REST_SERVER_USER_HANDLER_H_