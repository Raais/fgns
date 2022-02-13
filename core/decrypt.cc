#include "core_utils.h"

bool FGNS::decrypt(FGNS::Block &block, std::string target, std::string password, int mode)
{
    if (FGNS::exists(block, target, mode))
    {
        FGNS::File &file = *FGNS::getf_ptr(block, target, mode);

        if (file.ENCRYPTED)
        {
            if (!file.content.empty())
            {
                if (FGNS::AuthenticatePassword(password, file.HASH, file.SALT))
                {
                    CryptoPP::SecByteBlock key = FGNS::KDF(password, file.SALT);

                    file.content = FGNS::AESDecryptString(key, file.content);
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