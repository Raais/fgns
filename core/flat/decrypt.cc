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
            fprintf(stderr, "decrypt: cannot decrypt a directory\n");
            return false;
        }

        if (file.ENCRYPTED)
        {
            if (!file.content.empty())
            {
                if (FGNS::Crypto::AuthenticatePassword(password, file.HASH, file.SALT))
                {
                    CryptoPP::SecByteBlock key = FGNS::Crypto::KDF(password, file.SALT);

                    file.content = FGNS::Crypto::AESDecryptString(key, file.content);
                    file.ENCRYPTED = false;
                    file.HASH = "";
                    file.SALT = "";

                    return true;
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
        fprintf(stderr, "decrypt: file does not exist\n");
        return false;
    }
}