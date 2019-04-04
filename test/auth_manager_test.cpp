#include <gtest/gtest.h>
#include "auth_manager.hpp"
#include "auth_manager_test.hpp"

AuthManager AuthManagerTest::auth_manager_;

TEST_F(AuthManagerTest, SignOn) {
    UserInfo user{};
    
    bool result = auth_manager_.SignOn("admin@domain.com", "password", user);
    EXPECT_EQ(result, true);
    EXPECT_STREQ(user.username.c_str(), "adming@domain.com");
    EXPECT_STREQ(user.first_name.c_str(), "System");
    EXPECT_STREQ(user.last_name.c_str(), "Admin");
}