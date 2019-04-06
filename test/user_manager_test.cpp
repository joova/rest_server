#include "user_manager_test.hpp"

UserManager UserManagerTest::user_manager_;
std::string UserManagerTest::id_;

TEST_F(UserManagerTest, Create) {
    UserInfo user{};
    user.username = "test@test.com";
    user.password = "secret";
    user.first_name = "Ora";
    user.last_name = "Ngerti";

    id_ = user_manager_.Create(user);

    EXPECT_STRNE(id_.c_str(), "");
}

TEST_F(UserManagerTest, FindOne) {    
    UserInfo user = user_manager_.FindOne(id_);
    EXPECT_STREQ(user.username.c_str(), "test@test.com");
}

TEST_F(UserManagerTest, FindUsername) {    
    UserInfo user = user_manager_.FindUsername("test@test.com");
    EXPECT_STREQ(user.username.c_str(), "test@test.com");
}

TEST_F(UserManagerTest, Update) { 
    UserInfo user{};
    user.first_name = "Uwis";
    user.last_name = "Ngerti";

    int count = user_manager_.Update(id_, user);
    EXPECT_EQ(count, 1);
}

TEST_F(UserManagerTest, List) {
    std::vector<UserInfo> users = user_manager_.List();
    EXPECT_GE(users.size(), 0);
}

TEST_F(UserManagerTest, Count) {
    int64_t count = user_manager_.Count();
    std::vector<UserInfo> users = user_manager_.List();
    EXPECT_EQ(count, users.size());
}

TEST_F(UserManagerTest, FindText) {
    std::vector<UserInfo> users = user_manager_.Find(0,10,"hasyim");
    EXPECT_GE(users.size(), 0);
}

TEST_F(UserManagerTest, CountText) {
    int64_t count = user_manager_.Count("hasyim");
    EXPECT_GE(count, 0);
}

TEST_F(UserManagerTest, Delete) {    
    int count = user_manager_.Delete(id_);
    EXPECT_EQ(count, 1);
}
