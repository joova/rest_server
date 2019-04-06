#include <gtest/gtest.h>
#include <string>
#include "auth_manager.hpp"

class AuthManagerTest : public ::testing::Test {
protected:
    static void SetupTestSuite() {
        auth_manager_ = AuthManager{};
        id_ = std::string{};
    }

    static void TearDownTestSuite() {
        // delete user_manager_;
        // user_manager_ = NULL;
    }

    virtual void SetUp() {}
    virtual void TearDown() {}

    static AuthManager auth_manager_;
    static std::string id_;
};