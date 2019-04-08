#include <gtest/gtest.h>
#include "auth_handler.hpp"
#include "auth_handler_test.hpp"

AuthHandler AuthHandlerTest::auth_handler_;
std::string AuthHandlerTest::token_;

TEST_F(AuthHandlerTest, SignOn) {
    UserInfo user{};
    
    bool result = auth_handler_.SignOn("admin@domain.com", "password", user);
    EXPECT_EQ(result, true);
    EXPECT_STREQ(user.username.c_str(), "admin@domain.com");
    EXPECT_STREQ(user.first_name.c_str(), "System");
    EXPECT_STREQ(user.last_name.c_str(), "Admin");
}

TEST_F(AuthHandlerTest, CreateToken) {
    UserInfo user{};
    
    token_ = auth_handler_.CreateToken();
    EXPECT_STRNE(token_.c_str(), "");
}

TEST_F(AuthHandlerTest, VerifyToken) {
    UserInfo user{};
    
    bool result = auth_handler_.VerifyToken(token_);
    EXPECT_EQ(result, true);
}