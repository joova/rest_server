#include <cpprest/http_msg.h>

using namespace web;
using namespace http;

class Controller {
    public:
        virtual void HandleGet(http_request request) = 0;
        virtual void HandlePost(http_request request) = 0;
        virtual void HandlePut(http_request request) = 0;
        virtual void HandleDelete(http_request request) = 0;
};