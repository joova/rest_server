#include "controller.hpp"
#include "basic_controller.hpp"

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