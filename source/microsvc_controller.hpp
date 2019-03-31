#include "controller.hpp"
#include "basic_controller.hpp"

class MicroServiceController : public BasicController, Controller {
public:
    MicroServiceController() : BasicController() {}
    ~MicroServiceController() {}

    void handleGet(http_request request) override;
    void handlePost(http_request request) override;
    void handlePut(http_request request) override;
    void handleDelete(http_request request) override;
    
    void initRestHandlers() override;

private:
    static json::value respNotImpl(const http::method & method);
};