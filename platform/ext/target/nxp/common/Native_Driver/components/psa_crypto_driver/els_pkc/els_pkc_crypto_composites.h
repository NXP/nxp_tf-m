/*
 * Copyright 2023 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ELS_PKC_CRYPTO_COMPOSITES_H
#define ELS_PKC_CRYPTO_COMPOSITES_H

/** \file els_pkc_crypto_composites.h
 *
 * This file contains the declaration of the context structures related
 * to the els_pkc driver
 *
 */

/* Include CLNS header files */
#include <mcuxClPsaDriver_MemoryConsumption.h>
#include <mcuxClKey.h>
#include <mcuxClMac.h>
#include <mcuxClMacModes.h>
#include <mcuxClAead.h>

typedef struct {
    uint8_t clns_data[MCUXCLPSADRIVER_CLNSDATA_MAC_SIZE];
} els_pkc_transparent_mac_operation_t,els_pkc_opaque_mac_operation_t,els_pkc_mac_operation_t;

#define ELS_PKC_PSA_MAC_OPERATION_INIT { { 0 } }

typedef struct {
    psa_algorithm_t alg;
    psa_key_type_t key_type;
    uint8_t is_encrypt;
    uint8_t tag_length;

    uint32_t body_started;
    uint32_t ad_remaining;
    uint32_t body_remaining;
    uint32_t nonce_set;

    uint8_t clns_data[MCUXCLPSADRIVER_CLNSDATA_AEAD_SIZE];
} els_pkc_transparent_aead_operation_t,els_pkc_opaque_aead_operation_t,els_pkc_aead_operation_t;

#define ELS_PKC_PSA_AEAD_OPERATION_INIT { 0, 0, 0, 0, 0, 0, 0, 0, { 0 } }

#endif /* ELS_PKC_CRYPTO_COMPOSITES_H */
