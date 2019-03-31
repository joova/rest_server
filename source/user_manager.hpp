#include <string>
#include <vector>
#include "db_exception.hpp"

typedef struct {
    std::string username;
    std::string password;
    std::string firstName;
    std::string lastName;
} UserInfo;

class UserManger {
public:
    std::vector<UserInfo> list();
    void create(const UserInfo & user) throw(DbException);
    UserInfo find_one(std::string username) throw(DbException);
    std::vector<UserInfo> find(int offset, int limit);
    std::vector<UserInfo> find(int offset, int limit, std::string text);
};