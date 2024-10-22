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
#ifndef _PLAT_IAK_PK_H_
#define _PLAT_IAK_PK_H_

#if defined(PLATFORM_HAS_ATTEST_PK)

#include "pal_attestation_crypto.h"

/* Function to provide a potential implementation for devices which are 
using keys based upon secure element  during IAK */
extern int32_t tfm_initial_attest_get_public_key(uint8_t      *public_key_buff,
                                         size_t            public_key_buf_size,
                                         size_t           *public_key_len,
                                         psa_ecc_family_t *elliptic_family_type);
										 
#endif /* PLATFORM_HAS_ATTEST_PK */
#endif /* _PLAT_IAK_PK_H_ */