#pragma once

#include <string>
#include <cstring>

#ifdef __cplusplus
extern "C"{
#endif

#include <sodium.h>

#ifdef __cplusplus
}
#endif

#include "picosha2.hpp"

namespace StrCrypto
{
    std::string GenerateRandomBytes(size_t size);
    std::string SHA256Digest(const std::string& input);
    std::string KDF(std::string pwdstr, std::string sltstr);
    std::string HashPassword(std::string pwdstr);
           bool VerifyHash(std::string hashstr, std::string pwdstr);
    std::string AEADStringEncrypt(std::string &keystr, std::string &noncestr, const std::string &plainstr);
    std::string AEADStringDecrypt(std::string &keystr, std::string &noncestr, const std::string &cipherstr);

}