#include "core_utils.h"

bool FGNS::decrypt(FGNS::Block &block, std::string dst, std::string password, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
    {
        dst = FGNS::root_get_target_fuzzy(block, dst);
    }
    if (FGNS::exists(block, dst, mode))
    {
        FGNS::File &file = *FGNS::get_file_ptr(block, dst, mode);

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
                    std::cerr << "Password is incorrect" << std::endl;
                    return false;
                }
            }
            else
            {
                std::cerr << "File is empty" << std::endl;
                return false;
            }
        }
        else
        {
            std::cerr << "File is not encrypted" << std::endl;
            return false;
        }
    }
    else
    {
        std::cerr << "File does not exist" << std::endl;
        return false;
    }
}