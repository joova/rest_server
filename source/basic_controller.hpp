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