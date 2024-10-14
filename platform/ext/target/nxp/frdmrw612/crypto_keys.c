/*
 * Copyright (c) 2017-2022 Arm Limited. All rights reserved.
 * Copyright 2023 NXP
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include "fsl_device_registers.h"
#include "tfm_plat_crypto_keys.h"
#include "tfm_builtin_key_ids.h"
#include "tfm_plat_otp.h"
#include "psa_manifest/pid.h"
#include "tfm_builtin_key_loader.h"

#define NUMBER_OF_ELEMENTS_OF(x) sizeof(x)/sizeof(*x)
#define MAPPED_TZ_NS_AGENT_DEFAULT_CLIENT_ID -0x3c000000
#define TFM_NS_PARTITION_ID                  MAPPED_TZ_NS_AGENT_DEFAULT_CLIENT_ID

#ifndef MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER
#error "MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER must be selected in Mbed TLS config file"
#endif

#define PSA_KEY_LOCATION_S50_KEY_GEN_STORAGE ((psa_key_location_t)0xE00301)

#if USE_ELS_PKC_HUK

// The slot of S50 which the bootrom leaves the die-internal die-individual master key in
#define NXP_DIE_MK_SK_SLOT_INDEX             (0u)

static bool is_active_key_slot(mcuxClEls_KeyIndex_t keyIdx)
{
    mcuxClEls_KeyProp_t key_properties;
    key_properties.word.value = ((const volatile uint32_t *)(&ELS->ELS_KS0))[keyIdx];
    return key_properties.bits.kactv;
}

mcuxClEls_KeyIndex_t get_free_key_slot(uint32_t required_keyslots)
{
    for (mcuxClEls_KeyIndex_t keyIdx = 0; keyIdx <= (MCUXCLELS_KEY_SLOTS - required_keyslots); keyIdx++)
    {
        bool is_valid_keyslot = true;
        for (uint32_t i = 0; i < required_keyslots; i++)
        {
            if (is_active_key_slot(keyIdx + i))
            {
                is_valid_keyslot = false;
                break;
            }
        }

        if (is_valid_keyslot)
        {
            return keyIdx;
        }
    }
    return MCUXCLELS_KEY_SLOTS;
}

enum tfm_plat_err_t tfm_plat_get_huk_els_pkc(uint8_t *buf, size_t buf_len, size_t *key_len)
{
    mcuxClEls_KeyIndex_t keyIdx = 0;
    uint8_t key_buf[32]; /* 256-bit key. */

    // clang-format off
    const uint8_t label_buf[32] = {
          0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
          0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
          0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
          0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    };
    // clang-format on

    enum tfm_plat_err_t status = TFM_PLAT_ERR_SUCCESS;

    if ((buf == NULL) || (buf_len < sizeof(key_buf)))
    {
        status = TFM_PLAT_ERR_INVALID_INPUT;
        goto exit;
    }

    keyIdx = get_free_key_slot(2);
    if (!(keyIdx < MCUXCLCSS_KEY_SLOTS))
    {
        status = TFM_PLAT_ERR_SYSTEM_ERR;
        goto exit;
    }

    /* Derive a key using the NIST SP 800-108 CMAC-based Extract-and-Expand Key Derivation Function.*/
    const mcuxClEls_KeyProp_t targetKeyProperties = {
        .bits.upprot_priv = MCUXCLELS_KEYPROPERTY_PRIVILEGED_TRUE,
        .bits.upprot_sec  = MCUXCLELS_KEYPROPERTY_SECURE_TRUE,
        .bits.ksize       = MCUXCLELS_KEYPROPERTY_KEY_SIZE_256,
        .bits.uhkdf       = 1,
    }; /* Must be a 256-bit key with HKDF property bit set to 1. */
    const uint8_t DerivationData[MCUXCLCSS_CKDF_DERIVATIONDATA_SIZE] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6,
                                                                        0x7, 0x8, 0x9, 0xA, 0xB, 0xC};
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(
        resultCkdf, tokenCkdf,
        mcuxClEls_Ckdf_Sp800108_Async(NXP_DIE_MK_SK_SLOT_INDEX,
                                      keyIdx, /* Free slot - Key bank number of the derived key */
                                      targetKeyProperties, DerivationData));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Ckdf_Sp800108_Async) != tokenCkdf) ||
        (MCUXCLCSS_STATUS_OK_WAIT != resultCkdf))
    {
        status = TFM_PLAT_ERR_SYSTEM_ERR;
        goto exit;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(resultWaitCkdf, tokenWaitCkdf,
                                         mcuxClEls_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != tokenWaitCkdf) ||
        (MCUXCLCSS_STATUS_OK != resultWaitCkdf))
    {
        status = TFM_PLAT_ERR_SYSTEM_ERR;
        goto exit;
    }

    /* Derives a key using the HKDF (HMAC-based key derivation function) according to SP800-56C one-step approach
     * with Sha2-256. */
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(
        resultHkdf, tokenHkdf,
        mcuxClEls_Hkdf_Sp80056c_Async(keyIdx,    /* Key index used for derivation. */
                                      key_buf,   /* Memory area to store the derived key. Will be a 256-bit key. */
                                      label_buf, /* The derivation data */
                                      sizeof(label_buf)));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Hkdf_Sp80056c_Async) != tokenHkdf) ||
        (MCUXCLCSS_STATUS_OK_WAIT != resultHkdf))
    {
        status = TFM_PLAT_ERR_SYSTEM_ERR;
        goto exit;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(resultWaitHkdf, tokenWaitHkdf,
                                         mcuxClEls_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != tokenWaitHkdf) ||
        (MCUXCLCSS_STATUS_OK != resultWaitHkdf))
    {
        status = TFM_PLAT_ERR_SYSTEM_ERR;
        goto exit;
    }

    memcpy(buf, key_buf, sizeof(key_buf)); /* Copy key_size bytes to the key buffer. */
    *key_len = sizeof(key_buf);

exit:
    if (keyIdx < MCUXCLCSS_KEY_SLOTS && is_active_key_slot(keyIdx))
    {
        /* Delete the used key slot */
        MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(resultDel, tokenDel, mcuxClEls_KeyDelete_Async(keyIdx));
        if ((tokenDel != MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_KeyDelete_Async)) ||
            (resultDel != MCUXCLCSS_STATUS_OK_WAIT))
        {
            status = TFM_PLAT_ERR_SYSTEM_ERR;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(resultWaitDel, tokenWaitDel,
                                             mcuxClEls_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
        if ((tokenWaitDel != MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation)) ||
            (resultWaitDel != MCUXCLCSS_STATUS_OK))
        {
            status = TFM_PLAT_ERR_SYSTEM_ERR;
        }
    }
    return status;
}

#endif
//ST end

static enum tfm_plat_err_t tfm_plat_get_huk(uint8_t *buf, size_t buf_len,
                                            size_t *key_len,
                                            psa_key_bits_t *key_bits,
                                            psa_algorithm_t *algorithm,
                                            psa_key_type_t *type)
{
    enum tfm_plat_err_t err;
#if defined(PSA_CRYPTO_DRIVER_ELS_PKC) && USE_ELS_PKC_HUK
    err = tfm_plat_get_huk_els_pkc(buf, buf_len, key_len);
    if (err != TFM_PLAT_ERR_SUCCESS)
    {
        return err;
    }
#else
    err = tfm_plat_otp_read(PLAT_OTP_ID_HUK, buf_len, buf);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = tfm_plat_otp_get_size(PLAT_OTP_ID_HUK, key_len);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }
#endif

    *key_bits = *key_len * 8;
    *algorithm = PSA_ALG_HKDF(PSA_ALG_SHA_256);
    *type = PSA_KEY_TYPE_DERIVE;

    return TFM_PLAT_ERR_SUCCESS;
}

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
static enum tfm_plat_err_t tfm_plat_get_iak(uint8_t *buf, size_t buf_len,
                                            size_t *key_len,
                                            psa_key_bits_t *key_bits,
                                            psa_algorithm_t *algorithm,
                                            psa_key_type_t *type)
{
    enum tfm_plat_err_t err;
#ifndef SYMMETRIC_INITIAL_ATTESTATION
    psa_ecc_family_t curve_type;
#endif /* SYMMETRIC_INITIAL_ATTESTATION */

    err = tfm_plat_otp_read(PLAT_OTP_ID_IAK_LEN,
                            sizeof(size_t), (uint8_t*)key_len);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }
    *key_bits = *key_len * 8;

    if (buf_len < *key_len) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

#ifdef SYMMETRIC_INITIAL_ATTESTATION
    err = tfm_plat_otp_read(PLAT_OTP_ID_IAK_TYPE,
                            sizeof(psa_algorithm_t), (uint8_t*)algorithm);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    *type = PSA_KEY_TYPE_HMAC;
#else /* SYMMETRIC_INITIAL_ATTESTATION */
    err = tfm_plat_otp_read(PLAT_OTP_ID_IAK_TYPE, sizeof(psa_ecc_family_t),
                            &curve_type);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    *algorithm = PSA_ALG_ECDSA(PSA_ALG_SHA_256);
    *type = PSA_KEY_TYPE_ECC_KEY_PAIR(curve_type);
#endif /* SYMMETRIC_INITIAL_ATTESTATION */

    return tfm_plat_otp_read(PLAT_OTP_ID_IAK, *key_len, buf);
}
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
/**
 * @brief Table describing per-user key policy for the IAK
 *
 */
static const tfm_plat_builtin_key_per_user_policy_t g_iak_per_user_policy[] = {
    {.user = TFM_SP_INITIAL_ATTESTATION,
#ifdef SYMMETRIC_INITIAL_ATTESTATION
        .usage = PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_EXPORT,
#else
        .usage = PSA_KEY_USAGE_SIGN_HASH,
#endif /* SYMMETRIC_INITIAL_ATTESTATION */
    },
#ifdef TEST_S_ATTESTATION
    {.user = TFM_SP_SECURE_TEST_PARTITION, .usage = PSA_KEY_USAGE_VERIFY_HASH},
#endif /* TEST_S_ATTESTATION */
#ifdef TEST_NS_ATTESTATION
    {.user = TFM_NS_PARTITION_ID, .usage = PSA_KEY_USAGE_VERIFY_HASH},
#endif /* TEST_NS_ATTESTATION */
};
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

/**
 * @brief Table describing per-key user policies
 *
 */
static const tfm_plat_builtin_key_policy_t g_builtin_keys_policy[] = {
    {.key_id = TFM_BUILTIN_KEY_ID_HUK, .per_user_policy = 0, .usage = PSA_KEY_USAGE_DERIVE},
    {.key_id = TFM_BUILTIN_KEY_ID_EL2GO_CONN_AUTH, .per_user_policy = 0, .usage = PSA_KEY_USAGE_SIGN_HASH},
#ifdef TFM_PARTITION_INITIAL_ATTESTATION
    {.key_id = TFM_BUILTIN_KEY_ID_IAK,
     .per_user_policy = NUMBER_OF_ELEMENTS_OF(g_iak_per_user_policy),
     .policy_ptr = g_iak_per_user_policy},
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */
};

/**
 * @brief Table describing the builtin-in keys (plaform keys) available in the platform. Note
 *        that to bind the keys to the tfm_builtin_key_loader driver, the lifetime must be
 *        explicitly set to the one associated to the driver, i.e. TFM_BUILTIN_KEY_LOADER_LIFETIME
 */
static const tfm_plat_builtin_key_descriptor_t g_builtin_keys_desc[] = {
    {.key_id = TFM_BUILTIN_KEY_ID_HUK,
     .slot_number = TFM_BUILTIN_KEY_SLOT_HUK,
     .lifetime = TFM_BUILTIN_KEY_LOADER_LIFETIME,
     .loader_key_func = tfm_plat_get_huk},
     /* Would be nice to add EL2GO_CONN_AUTH change via pre-processor*/
    {.key_id = TFM_BUILTIN_KEY_ID_EL2GO_CONN_AUTH,
     .slot_number = 0U,
     .lifetime = PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(
                        PSA_KEY_LIFETIME_PERSISTENT,
                        PSA_KEY_LOCATION_S50_KEY_GEN_STORAGE),
     .loader_key_func = NULL/*tfm_plat_get_el2go_auth*/},
#ifdef TFM_PARTITION_INITIAL_ATTESTATION
    {.key_id = TFM_BUILTIN_KEY_ID_IAK,
     .slot_number = TFM_BUILTIN_KEY_SLOT_IAK,
#if defined(PSA_CRYPTO_DRIVER_ELS_PKC) && USE_ELS_PKC_IAK
     .lifetime = PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(
                        PSA_KEY_LIFETIME_PERSISTENT,
                        PSA_KEY_LOCATION_S50_KEY_GEN_STORAGE),
#else
     .lifetime = TFM_BUILTIN_KEY_LOADER_LIFETIME,
#endif /* USE_ELS_PKC_IAK */
     .loader_key_func = tfm_plat_get_iak},
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */
};

size_t tfm_plat_builtin_key_get_policy_table_ptr(const tfm_plat_builtin_key_policy_t *desc_ptr[])
{
    *desc_ptr = &g_builtin_keys_policy[0];
    return NUMBER_OF_ELEMENTS_OF(g_builtin_keys_policy);
}

size_t tfm_plat_builtin_key_get_desc_table_ptr(const tfm_plat_builtin_key_descriptor_t *desc_ptr[])
{
    *desc_ptr = &g_builtin_keys_desc[0];
    return NUMBER_OF_ELEMENTS_OF(g_builtin_keys_desc);
}
