#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <bcrypt.h>

class Crypto {
public:
    static std::string EncryptAES(const std::string& plaintext, const std::string& password, const std::string& salt);
    static std::string DecryptAES(const std::string& ciphertext, const std::string& password, const std::string& salt);
    static std::string GenerateSalt(size_t length = 16);
    static std::string HashPassword(const std::string& password, const std::string& salt);

private:
    static std::vector<BYTE> DeriveKey(const std::string& password, const std::string& salt, size_t keyLength = 32);
};