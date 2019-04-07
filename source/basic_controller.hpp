#ifndef REST_SERVER_BASIC_CONTROLLER_H_
#define REST_SERVER_BASIC_CONTROLLER_H_

#include <iostream>

#include <cpprest/http_listener.h>
#include <pplx/pplxtasks.h>

using namespace web;
using namespace http;
using namespace http::experimental::listener;
using namespace utility;

class BasicController {
    protected:
        http_listener _listener;

    public:
        BasicController();
        virtual ~BasicController();
    
    void SetEndpoint(const std::string &value);
    std::string endpoint() const;

    pplx::task<void> Accept();
    pplx::task<void> Shutdown();

    virtual void InitRestHandlers() { 
    }
    
    std::vector<utility::string_t> RequestPath(const http_request &request);
};

#endif // REST_SERVER_BASIC_CONTROLLER_H_s