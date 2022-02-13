#pragma once

#include <string>

#include "cryptlib.h"
#include "aes.h"
#include "sha.h"
#include "hex.h"
#include "hkdf.h"
#include "osrng.h"
#include "modes.h"
#include "filters.h"

namespace FGNS
{
namespace Crypto
{

std::string GenerateSalt();
std::string SHA256Digest(const std::string& input);
std::string HashPassword(std::string input, const std::string &salt);
bool AuthenticatePassword(std::string input, const std::string &auth_hash, const std::string &salt);
CryptoPP::SecByteBlock KDF(std::string password, std::string iv);
std::string AESEncryptString(CryptoPP::SecByteBlock &key, const std::string &str);
std::string AESDecryptString(CryptoPP::SecByteBlock &key, const std::string &str);

}
}