#include "str_crypto.hpp"

//randombytes_buf
std::string StrCrypto::GenerateRandomBytes(size_t size)
{
  unsigned char bytes[size];
  randombytes_buf(bytes, size);
  return std::string(reinterpret_cast<char*>(bytes), size);
}

//SHA256 Hash
std::string StrCrypto::SHA256Digest(const std::string& input)
{
  std::vector<unsigned char> hash(picosha2::k_digest_size);
  picosha2::hash256(input.begin(), input.end(), hash.begin(), hash.end());
  return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
}

//Argon2 KDF
std::string StrCrypto::KDF(std::string pwdstr, std::string sltstr)
{
  char *password = (char *)pwdstr.data();
  size_t key_len = crypto_aead_xchacha20poly1305_ietf_KEYBYTES;

  unsigned char *salt = (unsigned char *)sltstr.data();
  unsigned char key[key_len];

  if (crypto_pwhash
      (key, sizeof key, password, strlen(password), salt,
       crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE,
       crypto_pwhash_ALG_DEFAULT) != 0) {
    return "_error_pwhash_outofmemory_";
  }

  return std::string((char *)key, key_len);
}

//Argon2 Password Storage
std::string StrCrypto::HashPassword(std::string pwdstr)
{
  char *password = (char *)pwdstr.data();
  char hashed_password[crypto_pwhash_STRBYTES];

  if (crypto_pwhash_str
      (hashed_password, password, strlen(password),
       crypto_pwhash_OPSLIMIT_SENSITIVE, crypto_pwhash_MEMLIMIT_SENSITIVE) != 0) {
    return "_error_pwhash_outofmemory_";
  }

  return std::string((char *)hashed_password, crypto_pwhash_STRBYTES);
}

//Argon2 Verification
bool StrCrypto::VerifyHash(std::string hashstr, std::string pwdstr)
{
  char *hash = (char *)hashstr.data();
  char *password = (char *)pwdstr.data();

  if (crypto_pwhash_str_verify
    (hash, password, strlen(password)) != 0) {
    return false;
  }
  else {
    return true;
  }
}

//XChaCha20-Poly1305 AEAD Encryption
std::string StrCrypto::AEADStringEncrypt(std::string &keystr, std::string &noncestr, const std::string &plainstr)
{
  const unsigned char *plaintext = (const unsigned char *)plainstr.data();
  size_t plaintext_len = plainstr.size();

  unsigned char *key = (unsigned char *)keystr.data();
  unsigned char *nonce = (unsigned char *)noncestr.data();

  unsigned char ciphertext[plaintext_len + crypto_aead_xchacha20poly1305_ietf_ABYTES];
  unsigned long long ciphertext_len;

  crypto_aead_xchacha20poly1305_ietf_encrypt(ciphertext, &ciphertext_len,
                                             plaintext, plaintext_len,
                                             nullptr, 0,
                                             NULL, nonce, key);

  return std::string((char *)ciphertext, ciphertext_len);
}

//XChaCha20-Poly1305 AEAD Decryption
std::string StrCrypto::AEADStringDecrypt(std::string &keystr, std::string &noncestr, const std::string &cipherstr)
{
  const unsigned char *ciphertext = (const unsigned char *)cipherstr.data();
  size_t ciphertext_len = cipherstr.size();

  unsigned char *key = (unsigned char *)keystr.data();
  unsigned char *nonce = (unsigned char *)noncestr.data();

  unsigned char decrypted[ciphertext_len];
  unsigned long long decrypted_len;
  if (crypto_aead_xchacha20poly1305_ietf_decrypt(decrypted, &decrypted_len,
                                               NULL,
                                               ciphertext, ciphertext_len,
                                               nullptr,
                                               0,
                                               nonce, key) != 0) {
    return std::string("_error_decrypt_messageforged_");
  }
  return std::string((char *)decrypted, decrypted_len);
}
