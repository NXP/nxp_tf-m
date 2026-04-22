#-------------------------------------------------------------------------------
# SPDX-FileCopyrightText: Copyright The TrustedFirmware-M Contributors
# Copyright 2026 NXP.
# 
# SPDX-License-Identifier: BSD-3-Clause
#-------------------------------------------------------------------------------

if(NOT DEFINED PLATFORM_DIR)
    set(PLATFORM_DIR ../../.. CACHE PATH "Path to plaform dir")
endif()

################################# BL2 ###########################################
if (BL2)

    target_compile_definitions(bl2_crypto
        PUBLIC
            # Since TF-M 2.2.x and MCUBoot 2.2.0 require BL2 HW crypto
            # accelerator to implement the PSA unified driver API, force
            # disabling of use of NXP HW accelerator in BL2 until they comply.
            MBEDTLS_ACCELERATOR_PSA_CRYPTO_CONFIG_FILE="${PLATFORM_DIR}/ext/accelerator/nxp/bl2_disabled_crypto_accelerator_config.h"
    )
endif()

# Platform specific NXP HW accel configs
include(${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/accelerator/CMakeLists.txt)

#common NXP HW accel configs
if (TFM_PARTITION_CRYPTO)

    target_link_libraries(crypto_service_mbedcrypto
        PUBLIC
            crypto_service_crypto_hw
    )

    target_compile_definitions(crypto_service_mbedcrypto
        PUBLIC
            $<$<BOOL:${CONFIG_PSA_CRYPTO_DRIVER_SGI_PKC}>:
            PSA_CRYPTO_DRIVER_SGI
            PSA_CRYPTO_DRIVER_PKC
            >
            MBEDTLS_ENTROPY_HARDWARE_ALT
    )
endif()
