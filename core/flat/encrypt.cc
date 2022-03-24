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
            std::string p_hash = StrCrypto::HashPassword(password);
            if (p_hash == "_error_pwhash_outofmemory_")
            {
                fprintf(stderr, "encrypt: ABORTED: OUT OF MEMORY\n");
                return false;
            }

            std::string k_salt = StrCrypto::GenerateRandomBytes(crypto_pwhash_SALTBYTES);
            std::string e_key = StrCrypto::KDF(password, k_salt);
            if (e_key == "_error_pwhash_outofmemory_")
            {
                fprintf(stderr, "encrypt: ABORTED: OUT OF MEMORY\n");
                return false;
            }
            std::string e_nonce = StrCrypto::GenerateRandomBytes(crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);

            file.content = StrCrypto::AEADStringEncrypt(e_key, e_nonce, file.content);
            file.content = p_hash + k_salt + e_nonce + file.content;

            block.SAVED = false;

            return true;
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