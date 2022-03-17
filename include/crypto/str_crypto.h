#pragma once

#include <string>

#include "cryptlib.h"
#include "aes.h"
#include "sha.h"
#include "hex.h"
#include "gcm.h"
#include "hkdf.h"
#include "osrng.h"
#include "modes.h"
#include "filters.h"

namespace FGNS
{
    namespace Crypto
    {
        std::string GenerateRandomBytes     (size_t size);
        std::string SHA512Digest     (const std::string& input);
        std::string SHA256Digest     (const std::string& input);
        CryptoPP::SecByteBlock KDF   (std::string password, std::string iv);
        std::string AESEncryptString (CryptoPP::SecByteBlock &key, std::string &iv, const std::string &str);
        std::string AESDecryptString (CryptoPP::SecByteBlock &key, std::string &iv, const std::string &str);
    }
}