#include <iostream>
#include "basic_controller.hpp"


//constructor
BasicController::BasicController(){
    std::cout << "constructor basic controller ..." << "\n";
}

//desctructor
BasicController::~BasicController(){
    std::cout << "destructor basic controller ..." << "\n";
}

//set endpoint
void BasicController::setEndpoint(const std::string & value) {
    uri endpointUri(value);
    _listener = http_listener(endpointUri);
}

//endpoint
std::string BasicController::endpoint() const {
    return _listener.uri().to_string();
}

//accept
pplx::task<void> BasicController::accept() {
    initRestHandlers();
    return _listener.open();
}

//shutdown
pplx::task<void> BasicController::shutdown() {
    return _listener.close();
}

std::vector<utility::string_t> BasicController::requestPath(const http_request & request){
    auto relativePath = uri::decode(request.relative_uri().path());
    return uri::split_path(relativePath);
}