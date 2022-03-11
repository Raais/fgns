#include "str_crypto.h"

std::string FGNS::Crypto::GenerateSalt()
{
  std::string salt;
  salt.resize(16);
  CryptoPP::AutoSeededRandomPool prng;
  prng.GenerateBlock((CryptoPP::byte*)salt.data(), salt.size());
  return salt;
}

std::string FGNS::Crypto::SHA512Digest(const std::string& input)
{
  std::string digest;
  CryptoPP::SHA512 sha512;
  CryptoPP::StringSource ss(input, true,
    new CryptoPP::HashFilter(sha512,
      new CryptoPP::HexEncoder(
        new CryptoPP::StringSink(digest),
        false
      )
    )
  );
  return digest;
}

std::string FGNS::Crypto::SHA256Digest(const std::string& input)
{
  std::string digest;

  CryptoPP::SHA256 sha256;
  CryptoPP::StringSource ss(input, true,
    new CryptoPP::HashFilter(sha256,
      new CryptoPP::HexEncoder(
        new CryptoPP::StringSink(digest),
        false
      )
    )
  );
  return digest;
}

std::string FGNS::Crypto::HashPassword(std::string input, const std::string &salt)
{
  input = input + salt;
  return FGNS::Crypto::SHA256Digest(input);
}

bool FGNS::Crypto::AuthenticatePassword(std::string input, const std::string &auth_hash, const std::string &salt)
{
  std::string input_hash = FGNS::Crypto::HashPassword(input, salt);
  return input_hash == auth_hash;
}

CryptoPP::SecByteBlock FGNS::Crypto::KDF(std::string password, std::string iv)
{
  CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH+CryptoPP::AES::BLOCKSIZE);
  CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
  hkdf.DeriveKey(key, key.size(), (const CryptoPP::byte*)password.data(), password.size(), (const CryptoPP::byte*)iv.data(), iv.size(), NULL, 0);
  return key;
}

std::string FGNS::Crypto::AESEncryptString(CryptoPP::SecByteBlock &key, const std::string &str)
{
  std::string ciphertext;
  CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption e;
  e.SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key+CryptoPP::AES::MAX_KEYLENGTH);
  CryptoPP::StringSource ss(str, true,
    new CryptoPP::StreamTransformationFilter(e,
      new CryptoPP::StringSink(ciphertext)
    )
  );
  return ciphertext;
}

std::string FGNS::Crypto::AESDecryptString(CryptoPP::SecByteBlock &key, const std::string &str)
{
  std::string plaintext;
  CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption d;
  d.SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key+CryptoPP::AES::MAX_KEYLENGTH);
  CryptoPP::StringSource ss(str, true,
    new CryptoPP::StreamTransformationFilter(d,
      new CryptoPP::StringSink(plaintext)
    )
  );
  return plaintext;
}