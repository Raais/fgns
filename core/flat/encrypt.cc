#include "flat_utils.h"

bool FGNS::Flat::encrypt(FGNS::Flat::Block &block, std::string dst, std::string password, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::Flat::get_target_fuzzy(block, dst);
        
    if (FGNS::Flat::exists(block, dst, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);
        if (file.DIRECTORY)
        {
            fprintf(stderr, "encrypt: '%s' is a directory\n", dst.c_str());
            return false;
        }

        if (!file.ENCRYPTED)
        {
            if (!file.content.empty())
            {
                file.SALT = FGNS::Crypto::GenerateRandomBytes(16);// for password & kdf

                file.HASH = FGNS::Crypto::SHA256Digest(password + file.SALT); // password

                CryptoPP::SecByteBlock key = FGNS::Crypto::KDF(password, file.SALT); // kdf

                std::string IV = FGNS::Crypto::SHA512Digest(password + file.SALT); // for encryption
                IV.resize(16);

                file.content = FGNS::Crypto::AESEncryptString(key, IV, file.content); // encryption
                file.ENCRYPTED = true;
                block.SAVED = false;

                return true;
            }
            else
            {
                fprintf(stderr, "encrypt: file is empty\n");
                return false;
            }
        }
        else
        {
            fprintf(stderr, "encrypt: file is already encrypted\n");
            return false;
        }
    }
    else
    {
        fprintf(stderr, "encrypt: '%s' does not exist\n", dst.c_str());
        return false;
    }
}