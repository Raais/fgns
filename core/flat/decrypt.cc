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

        if (file.content.substr(0, 9) == "$argon2id")
        {
            if (!file.content.empty())
            {
                std::string ciphertext = file.content;
                std::string phash = ciphertext.substr(0, crypto_pwhash_STRBYTES);
                if (StrCrypto::VerifyHash(phash, password))
                {
                    std::string ksalt = ciphertext.substr(crypto_pwhash_STRBYTES, crypto_pwhash_SALTBYTES);
                    std::string enonce = ciphertext.substr(crypto_pwhash_STRBYTES + crypto_pwhash_SALTBYTES,
                        crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);

                    std::string ekey = StrCrypto::KDF(password, ksalt);
                    ciphertext = ciphertext.substr(crypto_pwhash_STRBYTES + crypto_pwhash_SALTBYTES
                        + crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);


                    std::string decrypted = StrCrypto::AEADStringDecrypt(ekey, enonce, ciphertext);

                    if (decrypted == "DECRYPTION_ERROR_MESSAGE_FORGED")
                    {
                        fprintf(stderr, "decrypt:%s\n", decrypted.substr(16).c_str());
                        return false;
                    }
                    else
                    {
                        file.content = decrypted;
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