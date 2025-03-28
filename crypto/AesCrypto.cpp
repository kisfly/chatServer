//
// Created by subingwen
//

#include "AesCrypto.h"
#include "Hash.h"
#include"Base64.h"
#include <assert.h>
#include <openssl/aes.h>
#include<openssl/err.h>
#include <openssl/rand.h>

AesCrypto::AesCrypto(Algorithm algorithm, string key)
{
    switch(algorithm)
    {
        case AES_CBC_128:
        case AES_ECB_128:
        case AES_OFB_128:
        case AES_CFB_128:
        case AES_CTR_128:
            assert(key.size() == 16);
            break;
        case AES_CBC_192:
        case AES_ECB_192:
        case AES_OFB_192:
        case AES_CFB_192:
        case AES_CTR_192:
            assert(key.size() == 24);
            break;
        case AES_CBC_256:
        case AES_ECB_256:
        case AES_OFB_256:
        case AES_CFB_256:
        case AES_CTR_256:
            assert(key.size() == 32);
            break;
    }

    m_type = algorithm;
    //generateAESKey(m_key);
    m_key = key;
    //m_key="12345678876543211234567887654321";
}

AesCrypto::~AesCrypto()
{

}

string AesCrypto::encrypt(string text)
{
    return aesCrypto(text, ENCRYPTO);
}

string AesCrypto::decrypt(string text)
{
    return aesCrypto(text, DECRYPTO);
}

string AesCrypto::aesCrypto(string text, CryptoType type)
{    
    if(type==ENCRYPTO)
    {        
        size_t ciphertext_len = text.size(); // 从输入参数或数据源获取
        //int block_size = EVP_CIPHER_block_size(EVP_aes_256_cbc()); // 获取块大小（通常为16字节）
        size_t remainder = ciphertext_len % AES_BLOCK_SIZE;     
    }
    else
    {
        Base64 base;
        text=base.decode(text); 
    }
    unsigned char ivec[AES_BLOCK_SIZE];
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    assert(ctx);
    generateIvec(ivec);
    int ret = EVP_CipherInit_ex(ctx, m_algorithms.at(m_type)(),
                                NULL, reinterpret_cast<unsigned char*>(m_key.data()),
                                ivec, type);
    assert(ret);
    // 准备存储数据的内存
    int length  = text.size() + 1;
    if(length % AES_BLOCK_SIZE)
    {
        length = (length / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
    }
    unsigned char *out = new unsigned char[length];
    int outLen = 0, totalLen = 0;

    if(type==DECRYPTO)
    {
        //LOG_INFO<<"解密之前的字符串："<<text<<"\n";
        // 假设 ciphertext 是密文数据，ciphertext_len 是密文长度
    size_t ciphertext_len = text.size(); // 从输入参数或数据源获取
    //int block_size = EVP_CIPHER_block_size(EVP_aes_256_cbc()); // 获取块大小（通常为16字节）
    size_t remainder = ciphertext_len % AES_BLOCK_SIZE;
    }
    
    

    ret = EVP_CipherUpdate(ctx, out, &outLen,
                           reinterpret_cast<const unsigned char *>(text.data()), text.size());
    totalLen += outLen;
    assert(ret);

    ret = EVP_CipherFinal_ex(ctx, out + outLen, &outLen);
    totalLen += outLen;
    if (ret != 1) {
        unsigned long err = ERR_get_error();
        const char* lib = ERR_lib_error_string(err);
        const char* reason = ERR_reason_error_string(err);
        EVP_CIPHER_CTX_free(ctx);
        return ""; // 返回空或抛出异常
    }

    string outtext(reinterpret_cast<char *>(out), totalLen);
    if(type==ENCRYPTO)
    {
        //LOG_INFO<<"加密之后的字符串："<<outtext<<"\n";
        size_t ciphertext_len = outtext.size(); // 从输入参数或数据源获取
        //int block_size = EVP_CIPHER_block_size(EVP_aes_256_cbc()); // 获取块大小（通常为16字节）
        size_t remainder = ciphertext_len % AES_BLOCK_SIZE;

        Base64 base;
        outtext=base.encode(outtext); 
    }
    if(type==DECRYPTO)
    {
        // 假设 ciphertext 是密文数据，ciphertext_len 是密文长度
    size_t ciphertext_len = outtext.size(); // 从输入参数或数据源获取
    //int block_size = EVP_CIPHER_block_size(EVP_aes_256_cbc()); // 获取块大小（通常为16字节）
    size_t remainder = ciphertext_len % AES_BLOCK_SIZE;

    }
    delete[]out;
    EVP_CIPHER_CTX_free(ctx);
    return outtext;
}
bool AesCrypto::generateAESKey(std::string& key) {
    const int KEY_LENGTH = 32;  // AES-256需要32字节
    unsigned char key_buf[KEY_LENGTH];
    
    if (RAND_bytes(key_buf, KEY_LENGTH) != 1) {  // 关键安全接口
        unsigned long err = ERR_get_error();
        return false;
    }
    
    key.assign(reinterpret_cast<char*>(key_buf), KEY_LENGTH);
    return true;
}

void AesCrypto::generateIvec(unsigned char *ivec)
{
    Hash hs(HashType::Sha3_256);
    hs.addData(m_key);
    std::string res = hs.result(Hash::Type::Binary);
    for(int i=0; i<AES_BLOCK_SIZE; ++i)
    {
        ivec[i] = res.at(i);
    }
}