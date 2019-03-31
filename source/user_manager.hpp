#include <string>
#include <vector>

typedef struct {
    std::string username;
    std::string password;
    std::string firstName;
    std::string lastName;
} UserInfo;

class UserManger {
public:
    std::vector<UserInfo> list();
    void create(const UserInfo & user);
};