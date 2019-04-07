#include <gtest/gtest.h>
#include <string>
#include "auth_handler.hpp"

class AuthHandlerTest : public ::testing::Test {
protected:
    static void SetupTestSuite() {
        auth_handler_ = AuthHandler{};
        id_ = std::string{};
    }

    static void TearDownTestSuite() {
        // delete user_manager_;
        // user_manager_ = NULL;
    }

    virtual void SetUp() {}
    virtual void TearDown() {}

    static AuthHandler auth_handler_;
    static std::string id_;
};