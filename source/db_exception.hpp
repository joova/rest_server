#include <exception>
#include <string>

// Database Exception classs
class DbException : public std::exception {
    std::string _message;
public:
    DbException(const std::string& message) : _message(message){}
    
    const char* what() const throw() {
        return _message.c_str();
    }
};