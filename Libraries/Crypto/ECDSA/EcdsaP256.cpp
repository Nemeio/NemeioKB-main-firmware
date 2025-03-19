#include "EcdsaP256.hpp"
#include "assertTools.h"
#include "crypto.h"
#include "AutoLock.h"

static const uint8_t P_256_a[] = {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
                                  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC};
/* coefficient b */
static const uint8_t P_256_b[] = {0x5a, 0xc6, 0x35, 0xd8, 0xaa, 0x3a, 0x93, 0xe7, 0xb3, 0xeb, 0xbd,
                                  0x55, 0x76, 0x98, 0x86, 0xbc, 0x65, 0x1d, 0x06, 0xb0, 0xcc, 0x53,
                                  0xb0, 0xf6, 0x3b, 0xce, 0x3c, 0x3e, 0x27, 0xd2, 0x60, 0x4b};

/* prime modulus p*/
static const uint8_t P_256_p[] = {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
                                  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/* order n*/
static const uint8_t P_256_n[] = {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
                                  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBC, 0xE6, 0xFA, 0xAD, 0xA7, 0x17,
                                  0x9E, 0x84, 0xF3, 0xB9, 0xCA, 0xC2, 0xFC, 0x63, 0x25, 0x51};

/* base point Gx*/
static const uint8_t P_256_Gx[] = {0x6B, 0x17, 0xD1, 0xF2, 0xE1, 0x2C, 0x42, 0x47, 0xF8, 0xBC, 0xE6,
                                   0xE5, 0x63, 0xA4, 0x40, 0xF2, 0x77, 0x03, 0x7D, 0x81, 0x2D, 0xEB,
                                   0x33, 0xA0, 0xF4, 0xA1, 0x39, 0x45, 0xD8, 0x98, 0xC2, 0x96};

/* base point Gy*/
static const uint8_t P_256_Gy[] = {0x4F, 0xE3, 0x42, 0xE2, 0xFE, 0x1A, 0x7F, 0x9B, 0x8E, 0xE7, 0xEB,
                                   0x4A, 0x7C, 0x0F, 0x9E, 0x16, 0x2B, 0xCE, 0x33, 0x57, 0x6B, 0x31,
                                   0x5E, 0xCE, 0xCB, 0xB6, 0x40, 0x68, 0x37, 0xBF, 0x51, 0xF5};

EcdsaP256::EcdsaP256(etl::array_view<const std::byte> publicKey, ICRC32& crc32)
    : m_publicKey(publicKey)
    , m_crc32(crc32)
{
    ASSERT(m_publicKey.size() == 64);
}

bool EcdsaP256::verify(etl::array_view<const std::byte> digest,
                       etl::array_view<const std::byte> signature)
{
    if(digest.size() != CRL_SHA256_SIZE || signature.size() != 64)
    {
        return false;
    }

    AutoLock crcLock(m_crc32);

    bool ret = false;

    ECDSAsignature_stt* sign = nullptr;
    /* Proceed with ECC signature generation */
    EC_stt EC_st;
    /* structure to store the preallocated buffer for computation*/
    membuf_stt Crypto_Buffer;

    /* Initialize the ECC curve structure, with all the parameters */
    EC_st.pmA = P_256_a;
    EC_st.pmB = P_256_b;
    EC_st.pmP = P_256_p;
    EC_st.pmN = P_256_n;
    EC_st.pmGx = P_256_Gx;
    EC_st.pmGy = P_256_Gy;
    EC_st.mAsize = sizeof(P_256_a);
    EC_st.mBsize = sizeof(P_256_b);
    EC_st.mNsize = sizeof(P_256_n);
    EC_st.mPsize = sizeof(P_256_p);
    EC_st.mGxsize = sizeof(P_256_Gx);
    EC_st.mGysize = sizeof(P_256_Gy);
    auto pub_x = reinterpret_cast<const uint8_t*>(m_publicKey.data());
    auto pub_y = reinterpret_cast<const uint8_t*>(m_publicKey.data() + 32);
    auto sign_r = reinterpret_cast<const uint8_t*>(signature.data());
    auto sign_s = reinterpret_cast<const uint8_t*>(signature.data() + 32);

    /* We prepare the memory buffer strucure */
    Crypto_Buffer.pmBuf = reinterpret_cast<uint8_t*>(m_preallocatedBuffer.data());
    Crypto_Buffer.mUsed = 0;
    Crypto_Buffer.mSize = m_preallocatedBuffer.size();

    /* Init the Elliptic Curve, passing the required memory */
    /* Note: This is not a temporary buffer, the memory inside EC_stt_buf is linked to EC_st *
         As long as there's need to use EC_st the content of EC_stt_buf should not be altered */
    int32_t status = ECCinitEC(&EC_st, &Crypto_Buffer);

    ECpoint_stt* PubKey = nullptr;
    if(status == ECC_SUCCESS)
    {
        /* EC is initialized, now prepare to import the public key. First, allocate a point */
        status = ECCinitPoint(&PubKey, &EC_st, &Crypto_Buffer);
    }

    if(status == ECC_SUCCESS)
    {
        /* Point is initialized, now import the public key */
        ECCsetPointCoordinate(PubKey, E_ECC_POINT_COORDINATE_X, pub_x, 32);
        ECCsetPointCoordinate(PubKey, E_ECC_POINT_COORDINATE_Y, pub_y, 32);

        /* Try to validate the Public Key. */
        status = ECCvalidatePubKey(PubKey, &EC_st, &Crypto_Buffer);
    }

    if(status == ECC_SUCCESS)
    {
        /* Public Key is validated, Initialize the signature object */
        status = ECDSAinitSign(&sign, &EC_st, &Crypto_Buffer);
    }

    if(status == ECC_SUCCESS)
    {
        /* Import the signature values */
        (void) ECDSAsetSignature(sign, E_ECDSA_SIGNATURE_R_VALUE, sign_r, 32);
        (void) ECDSAsetSignature(sign, E_ECDSA_SIGNATURE_S_VALUE, sign_s, 32);

        /* Prepare the structure for the ECDSA signature verification */
        ECDSAverifyCtx_stt verctx;

        verctx.pmEC = &EC_st;
        verctx.pmPubKey = PubKey;

        /* Verify it */
        status = ECDSAverify(reinterpret_cast<const uint8_t*>(digest.data()),
                             digest.size(),
                             sign,
                             &verctx,
                             &Crypto_Buffer);
        if(status == SIGNATURE_VALID)
        {
            ret = true;
        }
        /* release resource ...*/
        (void) ECDSAfreeSign(&sign, &Crypto_Buffer);
        (void) ECCfreePoint(&PubKey, &Crypto_Buffer);
        (void) ECCfreeEC(&EC_st, &Crypto_Buffer);
    }

    return ret;
}