#include "Crypto.h"
#include <stdexcept>

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

std::vector<BYTE> Crypto::DeriveKey(const std::string& password, const std::string& salt, size_t keyLength) {
    BCRYPT_ALG_HANDLE hAlg = NULL;
    if (!NT_SUCCESS(BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, NULL, BCRYPT_ALG_HANDLE_HMAC_FLAG))) {
        throw std::runtime_error("Failed to open algorithm provider");
    }

    std::vector<BYTE> derivedKey(keyLength);
    if (!NT_SUCCESS(BCryptDeriveKeyPBKDF2(hAlg,
        (PUCHAR)password.data(), (ULONG)password.size(),
        (PUCHAR)salt.data(), (ULONG)salt.size(),
        100000, derivedKey.data(), (ULONG)keyLength, 0))) {
        BCryptCloseAlgorithmProvider(hAlg, 0);
        throw std::runtime_error("Failed to derive key");
    }

    BCryptCloseAlgorithmProvider(hAlg, 0);
    return derivedKey;
}

std::string Crypto::GenerateSalt(size_t length) {
    BCRYPT_ALG_HANDLE hAlg = NULL;
    if (!NT_SUCCESS(BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_RNG_ALGORITHM, NULL, 0))) {
        throw std::runtime_error("Failed to open RNG algorithm provider");
    }

    std::vector<BYTE> salt(length);
    if (!NT_SUCCESS(BCryptGenRandom(hAlg, salt.data(), (ULONG)length, 0))) {
        BCryptCloseAlgorithmProvider(hAlg, 0);
        throw std::runtime_error("Failed to generate random bytes");
    }

    BCryptCloseAlgorithmProvider(hAlg, 0);
    
    std::string hexStr;
    const char hexChars[] = "0123456789abcdef";
    for (BYTE b : salt) {
        hexStr.push_back(hexChars[b >> 4]);
        hexStr.push_back(hexChars[b & 0x0F]);
    }
    return hexStr;
}

static std::vector<BYTE> HexToBytes(const std::string& hex) {
    std::vector<BYTE> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        BYTE byte = (BYTE)strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

std::string Crypto::HashPassword(const std::string& password, const std::string& salt) {
    std::vector<BYTE> saltBytes = HexToBytes(salt);
    std::string realSalt = std::string((char*)saltBytes.data(), saltBytes.size());
    std::vector<BYTE> key = DeriveKey(password, realSalt, 32);
    
    std::string hexStr;
    const char hexChars[] = "0123456789abcdef";
    for (BYTE b : key) {
        hexStr.push_back(hexChars[b >> 4]);
        hexStr.push_back(hexChars[b & 0x0F]);
    }
    return hexStr;
}

std::string Crypto::EncryptAES(const std::string& plaintext, const std::string& password, const std::string& salt) {
    std::vector<BYTE> saltBytes = HexToBytes(salt);
    std::string realSalt = std::string((char*)saltBytes.data(), saltBytes.size());
    std::vector<BYTE> key = DeriveKey(password, realSalt, 32);
    
    BCRYPT_ALG_HANDLE hAlg = NULL;
    if (!NT_SUCCESS(BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_AES_ALGORITHM, NULL, 0))) {
        throw std::runtime_error("Failed to open AES algorithm provider");
    }

    if (!NT_SUCCESS(BCryptSetProperty(hAlg, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_CBC, sizeof(BCRYPT_CHAIN_MODE_CBC), 0))) {
        BCryptCloseAlgorithmProvider(hAlg, 0);
        throw std::runtime_error("Failed to set chaining mode");
    }

    BCRYPT_KEY_HANDLE hKey = NULL;
    if (!NT_SUCCESS(BCryptGenerateSymmetricKey(hAlg, &hKey, NULL, 0, key.data(), (ULONG)key.size(), 0))) {
        BCryptCloseAlgorithmProvider(hAlg, 0);
        throw std::runtime_error("Failed to generate symmetric key");
    }

    std::string ivStr = GenerateSalt(16);
    std::vector<BYTE> iv = HexToBytes(ivStr);

    ULONG cbData = 0;
    std::vector<BYTE> ivCopy = iv;
    if (!NT_SUCCESS(BCryptEncrypt(hKey, (PUCHAR)plaintext.data(), (ULONG)plaintext.size(), NULL, ivCopy.data(), (ULONG)ivCopy.size(), NULL, 0, &cbData, BCRYPT_BLOCK_PADDING))) {
        BCryptDestroyKey(hKey);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        throw std::runtime_error("Failed to get encrypt buffer size");
    }

    std::vector<BYTE> ciphertext(cbData);
    if (!NT_SUCCESS(BCryptEncrypt(hKey, (PUCHAR)plaintext.data(), (ULONG)plaintext.size(), NULL, iv.data(), (ULONG)iv.size(), ciphertext.data(), (ULONG)ciphertext.size(), &cbData, BCRYPT_BLOCK_PADDING))) {
        BCryptDestroyKey(hKey);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        throw std::runtime_error("Encryption failed");
    }

    BCryptDestroyKey(hKey);
    BCryptCloseAlgorithmProvider(hAlg, 0);

    std::string result = ivStr + ":";
    const char hexChars[] = "0123456789abcdef";
    for (BYTE b : ciphertext) {
        result.push_back(hexChars[b >> 4]);
        result.push_back(hexChars[b & 0x0F]);
    }
    return result;
}

std::string Crypto::DecryptAES(const std::string& ciphertext, const std::string& password, const std::string& salt) {
    size_t sep = ciphertext.find(':');
    if (sep == std::string::npos) throw std::runtime_error("Invalid ciphertext format");

    std::string ivStr = ciphertext.substr(0, sep);
    std::string encryptedHex = ciphertext.substr(sep + 1);

    std::vector<BYTE> saltBytes = HexToBytes(salt);
    std::string realSalt = std::string((char*)saltBytes.data(), saltBytes.size());
    std::vector<BYTE> key = DeriveKey(password, realSalt, 32);
    
    std::vector<BYTE> iv = HexToBytes(ivStr);
    std::vector<BYTE> encBytes = HexToBytes(encryptedHex);

    BCRYPT_ALG_HANDLE hAlg = NULL;
    if (!NT_SUCCESS(BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_AES_ALGORITHM, NULL, 0))) {
        throw std::runtime_error("Failed to open AES algorithm provider");
    }

    if (!NT_SUCCESS(BCryptSetProperty(hAlg, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_CBC, sizeof(BCRYPT_CHAIN_MODE_CBC), 0))) {
        BCryptCloseAlgorithmProvider(hAlg, 0);
        throw std::runtime_error("Failed to set chaining mode");
    }

    BCRYPT_KEY_HANDLE hKey = NULL;
    if (!NT_SUCCESS(BCryptGenerateSymmetricKey(hAlg, &hKey, NULL, 0, key.data(), (ULONG)key.size(), 0))) {
        BCryptCloseAlgorithmProvider(hAlg, 0);
        throw std::runtime_error("Failed to generate symmetric key");
    }

    ULONG cbData = 0;
    std::vector<BYTE> ivCopy = iv;
    if (!NT_SUCCESS(BCryptDecrypt(hKey, encBytes.data(), (ULONG)encBytes.size(), NULL, ivCopy.data(), (ULONG)ivCopy.size(), NULL, 0, &cbData, BCRYPT_BLOCK_PADDING))) {
        BCryptDestroyKey(hKey);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        throw std::runtime_error("Failed to get decrypt buffer size. Password may be incorrect.");
    }

    std::vector<BYTE> plaintext(cbData);
    if (!NT_SUCCESS(BCryptDecrypt(hKey, encBytes.data(), (ULONG)encBytes.size(), NULL, iv.data(), (ULONG)iv.size(), plaintext.data(), (ULONG)plaintext.size(), &cbData, BCRYPT_BLOCK_PADDING))) {
        BCryptDestroyKey(hKey);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        throw std::runtime_error("Decryption failed. Password may be incorrect.");
    }

    BCryptDestroyKey(hKey);
    BCryptCloseAlgorithmProvider(hAlg, 0);

    return std::string((char*)plaintext.data(), cbData);
}