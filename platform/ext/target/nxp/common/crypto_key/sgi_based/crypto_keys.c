/*
 * SPDX-FileCopyrightText: Copyright The TrustedFirmware-M Contributors
 * Copyright 2026 NXP
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
#include "tfm_plat_crypto_keys.h"
#include "tfm_builtin_key_ids.h"
#include "tfm_plat_otp.h"
#include "psa_manifest/pid.h"
#include "tfm_builtin_key_loader.h"

#if USE_SGI_HUK
#include <mcuxClSession.h>
#include <mcuxClKey.h>
#include <mcuxClKdfModes.h>
#include <mcuxClAes.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClBuffer.h>
#include <mcuxClExample_Session_Helper.h>
#include <mcuxClExample_RNG_Helper.h>
#endif /* USE_SGI_HUK */

#define NUMBER_OF_ELEMENTS_OF(x) sizeof(x)/sizeof(*x)
#define MAPPED_TZ_NS_AGENT_DEFAULT_CLIENT_ID -0x3c000000
#define TFM_NS_PARTITION_ID                  MAPPED_TZ_NS_AGENT_DEFAULT_CLIENT_ID

/* -------------------------------------------------------------------------- */
/* SGI low-level API placeholders                                              */
/* -------------------------------------------------------------------------- */
#if USE_SGI_HUK
static const uint8_t label[] = "NXP|TFM|HUK|CRYPTO|STORAGE";
static const uint8_t context[] = "HUK|AES-GCM-128";

/* -------------------------------------------------------------------------- */
/* Provision HUK into SGI slots 6/7                                            */
/* -------------------------------------------------------------------------- */
static enum tfm_plat_err_t tfm_plat_get_huk_via_sgi(uint8_t *buf, size_t buf_len, size_t *key_len)
{

    /**************************************************************************/
    /* Preparation  of key handle linked with SGI_KEY_SLOT 6                  */
    /**************************************************************************/
    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;

    /* Allocate and initialize session */
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLKEY_DERIVATION_CPU_WA_SIZE, 0U);

    /* Initialize the PRNG */
    MCUXCLEXAMPLE_INITIALIZE_PRNG(session);

    uint32_t keyHandleDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS] = {0u};
    MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
    mcuxClKey_Handle_t keyHandle = (mcuxClKey_Handle_t) &keyHandleDesc;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(kiKwk_status, kiKwk_token, mcuxClKey_init(
      /* mcuxClSession_Handle_t session:        */ session,
      /* mcuxClKey_Handle_t key:                */ keyHandle,
      /* mcuxClKey_Type_t type:                 */ mcuxClKey_Type_Aes256,
      /* uint8_t * pKeyData:                   */ (uint8_t *) NULL, /* not needed, key is already loaded */
      /* uint32_t keyDataLength:               */ 0u /* not needed, key is already loaded */)
    );

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != kiKwk_token) || (MCUXCLKEY_STATUS_OK != kiKwk_status))
    {
      return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**
     * Although the KWK data is already loaded into an SGI key slot,
     * call mcuxClKey_loadCopro to finish initialization of the key handle.
     * Use option MCUXCLKEY_LOADOPTION_ALREADYLOADED.
     *
     * For this example, the wrapping key is 256-bit and assumed in
     * MCUXCLKEY_LOADOPTION_SLOT_SGI_KEY_6.
     */

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(klKwk_status, klKwk_token, mcuxClKey_loadCopro(
      /* mcuxClSession_Handle_t session:      */ session,
      /* mcuxClKey_Handle_t key:              */ keyHandle,
      /* uint32_t loadOptions:               */ MCUXCLKEY_LOADOPTION_SLOT_SGI_KEY_6
                                                | MCUXCLKEY_LOADOPTION_ALREADYLOADED)
    );
    
    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_loadCopro) != klKwk_token) || (MCUXCLKEY_STATUS_OK != klKwk_status))
    {
      return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* Key Derivation                                                         */
    /**************************************************************************/

    /* Set up input parameter structures. */
    MCUXCLBUFFER_INIT_RO(labelBuf, session, label, sizeof(label));
    MCUXCLBUFFER_INIT_RO(contextBuf, session, context, sizeof(context));
    /* exclude '\0', subtract 1 from total size */
    struct mcuxClKey_DerivationInput inputLabel = {.input=labelBuf, .size=sizeof(label) - 1u}; 
    /* exclude '\0', subtract 1 from total size */
    struct mcuxClKey_DerivationInput inputContext = {.input=contextBuf, .size=sizeof(context) - 1u}; 

    mcuxClKey_DerivationInput_t inputs[] = {inputLabel, inputContext};

    /* Set up output structure. */
    uint8_t derivedKeyBuf[MCUXCLAES_AES256_KEY_SIZE];

    /* Create and initialize derivedKey descriptor structure. */
    uint32_t derivedKeyDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS] = {0u};
    MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
    mcuxClKey_Handle_t derivedKey = (mcuxClKey_Handle_t) &derivedKeyDesc;    
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultKeyInit2, tokenKeyInit2, mcuxClKey_init(
      /* mcuxClSession_Handle_t session         */ session,
      /* mcuxClKey_Handle_t key                 */ derivedKey,
      /* mcuxClKey_Type_t type                  */ mcuxClKey_Type_Aes256,
      /* uint8_t * pKeyData                    */ derivedKeyBuf,
      /* uint32_t keyDataLength                */ sizeof(derivedKeyBuf)
    ));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != tokenKeyInit2) || (MCUXCLKEY_STATUS_OK != resultKeyInit2))
    {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();
    
    /* Call key derivation function. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(resultDeriv, tokenDeriv, mcuxClKey_derivation(
      /* mcuxClSession_Handle_t pSession         */ session,
      /* mcuxClKey_Derivation_t derivationMode   */ mcuxClKdfModes_SP800_108_CM_CMAC, 
      /* mcuxClKey_Handle_t derivationKey        */ keyHandle,
      /* mcuxClKey_DerivationInput_t inputs[]    */ inputs,
      /* uint32_t numberOfInputs                */ 2u,
      /* mcuxClKey_Handle_t derivedKey           */ derivedKey
    ));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_derivation) != tokenDeriv) || (MCUXCLKEY_STATUS_OK != resultDeriv))
    {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Copy key_size bytes to the key buffer. */
    memcpy(buf, derivedKeyBuf, sizeof(derivedKeyBuf));
    *key_len = sizeof(derivedKeyBuf);

    /* clearing local key buffer */
    memset(derivedKeyBuf, 0u, sizeof(derivedKeyBuf));

    return TFM_PLAT_ERR_SUCCESS;
}
#endif /* USE_SGI_HUK */

/* ------------------------------------------------------------------------- */
/* HUK loader                                                                */
/* ------------------------------------------------------------------------- */
static enum tfm_plat_err_t tfm_plat_get_huk(uint8_t *buf, size_t buf_len,
                                            size_t *key_len,
                                            psa_key_bits_t *key_bits,
                                            psa_algorithm_t *algorithm,
                                            psa_key_type_t *type)
{
    enum tfm_plat_err_t err;

#if defined(PSA_CRYPTO_DRIVER_SGI) && USE_SGI_HUK
    err = tfm_plat_get_huk_via_sgi(buf, buf_len, key_len);
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
#ifdef TFM_PARTITION_INITIAL_ATTESTATION
    {.key_id = TFM_BUILTIN_KEY_ID_IAK,
     .slot_number = TFM_BUILTIN_KEY_SLOT_IAK,
     .lifetime = TFM_BUILTIN_KEY_LOADER_LIFETIME,
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
