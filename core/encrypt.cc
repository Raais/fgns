#include "core_utils.h"

bool FGNS::encrypt(FGNS::Block &block, std::string target, std::string password, int mode)
{
    if (FGNS::exists(block, target, mode))
    {
        FGNS::File &file = *FGNS::getf_ptr(block, target, mode);

        if (!file.ENCRYPTED)
        {
            if (!file.content.empty())
            {
                file.SALT = FGNS::GenerateSalt();
                file.HASH = FGNS::HashPassword(password, file.SALT);

                CryptoPP::SecByteBlock key = FGNS::KDF(password, file.SALT);

                file.content = FGNS::AESEncryptString(key, file.content);
                file.ENCRYPTED = true;

                return true;
            }
            else
            {
                std::cerr << "File is empty" << std::endl;
                return false;
            }
        }
        else
        {
            std::cerr << "File is already encrypted" << std::endl;
            return false;
        }
    }
    else
    {
        std::cerr << "File does not exist" << std::endl;
        return false;
    }
}