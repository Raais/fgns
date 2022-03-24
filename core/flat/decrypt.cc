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
            std::string ciphertext = file.content;
            std::string p_hash = ciphertext.substr(0, crypto_pwhash_STRBYTES);
            if (StrCrypto::VerifyHash(p_hash, password))
            {
                std::string k_salt = ciphertext.substr(crypto_pwhash_STRBYTES, crypto_pwhash_SALTBYTES);
                std::string e_nonce = ciphertext.substr(crypto_pwhash_STRBYTES + crypto_pwhash_SALTBYTES,
                    crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);

                std::string e_key = StrCrypto::KDF(password, k_salt);
                if (e_key == "_error_pwhash_outofmemory_")
                {
                    fprintf(stderr, "decrypt: ABORTED: OUT OF MEMORY\n");
                    return false;
                }
                ciphertext = ciphertext.substr(crypto_pwhash_STRBYTES + crypto_pwhash_SALTBYTES
                    + crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);


                std::string decrypted = StrCrypto::AEADStringDecrypt(e_key, e_nonce, ciphertext);

                if (decrypted == "_error_decrypt_messageforged_")
                {
                    fprintf(stderr, "decrypt: ABORTED: MESSAGE FORGED\n");
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