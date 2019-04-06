#ifndef REST_SERVER_CRYPTO_UTILS_H_
#define REST_SERVER_CRYPTO_UTILS_H_

#include <iostream>
#include <sstream>

#include "openssl/sha.h"


class CryptoUtils {
public:
    static std::string SHA256(std::string line);
private:
    static std::string toHex(unsigned char s);
};

#endif // REST_SERVER_CRYPTO_UTILS_H_
