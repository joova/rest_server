#include <cpprest/http_listener.h>
#include <pplx/pplxtasks.h>

using namespace web;
using namespace http;
using namespace http::experimental::listener;

class BasicController {
    protected:
        http_listener _listener;

    public:
        BasicController();
        virtual ~BasicController();
    
    void setEndpoint(const std::string &value);
    std::string endpoint() const;

    pplx::task<void> accept();
    pplx::task<void> shutdown();

    virtual void initRestHandlers() { 
    }
    
    std::vector<utility::string_t> requestPath(const http_request &request);
};