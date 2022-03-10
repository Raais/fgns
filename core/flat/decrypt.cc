#include "flat_utils.h"

bool FGNS::Flat::decrypt(FGNS::Flat::Block &block, std::string dst, std::string password, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::Flat::get_target_fuzzy(block.root, dst);

    if (FGNS::Flat::exists(block, dst, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);
        if (file.DIRECTORY)
        {
            fprintf(stderr, "Cannot decrypt a directory\n");
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
                    fprintf(stderr, "Wrong password\n");
                    return false;
                }
            }
            else
            {
                fprintf(stderr, "File is empty\n");
                return false;
            }
        }
        else
        {
            fprintf(stderr, "File is not encrypted\n");
            return false;
        }
    }
    else
    {
        fprintf(stderr, "File does not exist\n");
        return false;
    }
}