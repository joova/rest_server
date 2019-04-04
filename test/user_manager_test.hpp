#include <gtest/gtest.h>
#include <string>
#include "pool_config.hpp"
#include "db_pool_singleton.hpp"
#include "user_manager.hpp"

class UserManagerTest : public ::testing::Test {
protected:
    static void SetupTestSuite() {
        user_manager_ = UserManager{};
        id_ = std::string{};
    }

    static void TearDownTestSuite() {
        // delete user_manager_;
        // user_manager_ = NULL;
    }

    virtual void SetUp() {
    }
    virtual void TearDown() {}

    static UserManager user_manager_;
    static std::string id_;
};