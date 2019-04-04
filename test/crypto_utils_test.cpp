#include <gtest/gtest.h>
#include "crypto_utils.hpp"

TEST(CryptoUtilsTest, SHA256) {
    std::string hash = CryptoUtils::SHA256("password");
    EXPECT_STREQ(hash.c_str(), "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8");
}
