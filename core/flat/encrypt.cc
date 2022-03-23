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

        if (file.content.substr(0, 9) != "$argon2id")
        {
            if (!file.content.empty())
            {
                std::string phash = StrCrypto::HashPassword(password);

                std::string ksalt = StrCrypto::GenerateRandomBytes(crypto_pwhash_SALTBYTES);
                std::string ekey = StrCrypto::KDF(password, ksalt);
                std::string enonce = StrCrypto::GenerateRandomBytes(crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);

                file.content = StrCrypto::AEADStringEncrypt(ekey, enonce, file.content);
                file.content = phash + ksalt + enonce + file.content;

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