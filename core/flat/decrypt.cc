#include "flat_utils.h"

bool FGNS::Flat::decrypt(FGNS::Flat::Block &block, std::string dst, std::string password, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::Flat::get_target_fuzzy(block, dst);

    if (FGNS::Flat::exists(block, dst, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);
        if (file.DIRECTORY)
        {
            fprintf(stderr, "decrypt: '%s' is a directory\n", dst.c_str());
            return false;
        }

        if (file.ENCRYPTED)
        {
            if (!file.content.empty())
            {
                if (FGNS::Crypto::SHA256Digest(password + file.SALT) == file.HASH) // authenticate password match
                {
                    CryptoPP::SecByteBlock key = FGNS::Crypto::KDF(password, file.SALT); // kdf

                    std::string IV = FGNS::Crypto::SHA512Digest(password + file.SALT); // for decryption
                    IV.resize(16);

                    std::string decrypted = FGNS::Crypto::AESDecryptString(key, IV, file.content); // decryption

                    if (decrypted.substr(0, 16) == "DECRYPTION_ERROR")
                    {
                        fprintf(stderr, "decrypt:%s\n", decrypted.substr(16).c_str());
                        return false;
                    }
                    else
                    {
                        file.content = decrypted;
                        file.ENCRYPTED = false;
                        file.HASH = "";
                        file.SALT = "";
                        block.SAVED = false;

                        return true;
                    }
                }
                else
                {
                    fprintf(stderr, "decrypt: wrong password\n");
                    return false;
                }
            }
            else
            {
                fprintf(stderr, "decrypt: file is empty\n");
                return false;
            }
        }
        else
        {
            fprintf(stderr, "decrypt: file is not encrypted\n");
            return false;
        }
    }
    else
    {
        fprintf(stderr, "decrypt: '%s' does not exist\n", dst.c_str());
        return false;
    }
}