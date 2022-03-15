#include "str_crypto.h"

std::string FGNS::Crypto::GenerateRandomBytes(size_t size)
{
  std::string salt;
  salt.resize(size);
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
  return FGNS::Crypto::SHA512Digest(input);
}

bool FGNS::Crypto::AuthenticatePassword(std::string input, const std::string &auth_hash, const std::string &salt)
{
  std::string input_hash = FGNS::Crypto::HashPassword(input, salt);
  return input_hash == auth_hash;
}

CryptoPP::SecByteBlock FGNS::Crypto::KDF(std::string password, std::string iv)
{
  CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
  CryptoPP::HKDF<CryptoPP::SHA512> hkdf;
  hkdf.DeriveKey(key, key.size(), (const CryptoPP::byte*)password.data(), password.size(), (const CryptoPP::byte*)iv.data(), iv.size(), NULL, 0);
  return key;
}

std::string FGNS::Crypto::AESEncryptString(CryptoPP::SecByteBlock &key, std::string &iv, const std::string &str)
{
  std::string ciphertext;
  CryptoPP::GCM<CryptoPP::AES>::Encryption e;
  e.SetKeyWithIV(key, key.size(), (const CryptoPP::byte*)iv.data(), iv.size());
  CryptoPP::StringSource ss(str, true,
    new CryptoPP::AuthenticatedEncryptionFilter(e,
      new CryptoPP::StringSink(ciphertext), false, 12
    )
  );
  return ciphertext;
}

std::string FGNS::Crypto::AESDecryptString(CryptoPP::SecByteBlock &key, std::string &iv, const std::string &str)
{
  std::string plaintext;
  try
  {
    CryptoPP::GCM<CryptoPP::AES>::Decryption d;
    d.SetKeyWithIV(key, key.size(), (const CryptoPP::byte*)iv.data(), iv.size());
    CryptoPP::AuthenticatedDecryptionFilter df( d,
        new CryptoPP::StringSink(plaintext),
        CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS, 12
    );
    CryptoPP::StringSource ss(str, true,
      new CryptoPP::Redirector(df)
    );
    if (!df.GetLastResult())
    {
      return "DECRYPTION_ERROR error: data integrity fail";
    }
  }
  catch( CryptoPP::Exception& e )
  {
    return "DECRYPTION_ERROR error: " + std::string(e.what());
  }
  return plaintext;
}