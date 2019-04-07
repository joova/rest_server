#ifndef REST_SERVER_MICRO_SERVICE_CONTROLLER_H_
#define REST_SERVER_MICRO_SERVICE_CONTROLLER_H_

#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <cpprest/http_msg.h>

#include "controller.hpp"
#include "basic_controller.hpp"

#include "user_handler.hpp"
#include "auth_handler.hpp"
#include "health_handler.hpp"



class MicroServiceController : public BasicController, Controller {
public:
    MicroServiceController() : BasicController() {}
    ~MicroServiceController() {}

    void HandleGet(http_request request) override;
    void HandlePost(http_request request) override;
    void HandlePut(http_request request) override;
    void HandleDelete(http_request request) override;
    
    void InitRestHandlers() override;

private:
    static json::value respNotImpl(const http::method & method);
};

#endif // REST_SERVER_MICRO_SERVICE_CONTROLLER_H_