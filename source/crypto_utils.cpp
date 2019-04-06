#include <iomanip>
#include "crypto_utils.hpp"


std::string CryptoUtils::toHex(unsigned char s) {
    std::stringstream ss;
    ss << std::hex <<  std::setw(2) << std::setfill('0') << (int) s;
    return ss.str();
}   

std::string CryptoUtils::SHA256(std::string line) {    
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, line.c_str(), line.length());
    SHA256_Final(hash, &sha256);

    std::string output = "";    
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        output += toHex(hash[i]);
    }
    return output;
}