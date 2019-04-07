#ifndef REST_SERVER_HEALTH_HANDLER_H_
#define REST_SERVER_HEALTH_HANDLER_H_

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <pplx/pplxtasks.h>
#include <cpprest/http_msg.h>

using namespace web;
using namespace http;


class HealthHandler {
public:
    void HandleStatus(const http_request & request);
};

#endif // REST_SERVER_HEALTH_HANDLER_H_