#ifndef RSA_ENCRYPTOR_HPP
#define RSA_ENCRYPTOR_HPP

#include "mbedtls/pk.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"

#include <string>
#include <vector>



//THis class not used yet

class RsaEncryptor {
public:
    RsaEncryptor();
    ~RsaEncryptor();

    /**
     * @brief Initializes the RSA encryptor with a public key.
     * @param publicKeyPem The public key in PEM format.
     * @return 0 on success, or a Mbed TLS error code on failure.
     */
    int init(const std::string& publicKeyPem);

    int encrypt(const char *input, size_t inputLen, char *output, size_t outputSize, size_t *outputLen);

    std::string base64Encode(const unsigned char *data, size_t len);

    /**
     * @brief Encrypts data using the loaded public key.
     * @param input The data to encrypt.
     * @param output The encrypted data.
     * @return 0 on success, or a Mbed TLS error code on failure.
     */
    //int encrypt(const std::vector<unsigned char>& input, std::vector<unsigned char>& output);

private:
    mbedtls_pk_context pkCtx;
    mbedtls_entropy_context entropyCtx;
    mbedtls_ctr_drbg_context ctrDrbgCtx;
    bool initialized;

    void logMbedtlsError(int retCode, const char* functionName);
};

#endif // RSA_ENCRYPTOR_HPP