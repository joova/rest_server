#include "basic_controller.hpp"

using namespace utility;

//constructor
BasicController::BasicController(){
    std::cout << "constructor basic controller ..." << "\n";
}

//desctructor
BasicController::~BasicController(){
    std::cout << "destructor basic controller ..." << "\n";
}

//set endpoint
void BasicController::SetEndpoint(const std::string & value) {
    string_t u_value = conversions::to_string_t(value);
    uri endpointUri(u_value);
    _listener = http_listener(endpointUri);
}

//endpoint
std::string BasicController::endpoint() const {
    return conversions::to_utf8string(_listener.uri().to_string());
}

//accept
pplx::task<void> BasicController::Accept() {
    InitRestHandlers();
    return _listener.open();
}

//shutdown
pplx::task<void> BasicController::Shutdown() {
    return _listener.close();
}

std::vector<string_t> BasicController::RequestPath(const http_request & request){
    auto relative_path = uri::decode(request.relative_uri().path());
    return uri::split_path(relative_path);
}