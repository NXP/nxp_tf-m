#-------------------------------------------------------------------------------
# Copyright (c) 2020-2023, Arm Limited. All rights reserved.
# Copyright 2023-2024 NXP
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

################################## Dependencies ################################
#set(TFM_PLATFORM_NXP_HAL_FILE_PATH      "DOWNLOAD"      CACHE STRING    "Path to the NXP SDK hal (or DOWNLOAD to fetch automatically)")
#set(NXP_SDK_GIT_TAG                     "MCUX_2.15.0"   CACHE STRING    "The version of the NXP MCUXpresso SDK")

############################ Platform ##########################################
#set(PLATFORM_DEFAULT_ATTEST_HAL       OFF         CACHE BOOL      "Use default attest hal implementation.")
set(PLATFORM_DEFAULT_CRYPTO_KEYS       OFF         CACHE BOOL      "Use default crypto keys implementation")

############################ BL2 ########################################
#set(BL2_S_IMAGE_START                   "0x8000"        CACHE STRING    "Base address of the secure image in configuration with BL2")
#set(BL2_NS_IMAGE_START                  "0x30000"       CACHE STRING    "Base address of the non secure image in configuration with BL2")

# Platform-specific configurations
set(CONFIG_TFM_USE_TRUSTZONE            ON              CACHE BOOL      "Enable use of TrustZone to transition between NSPE and SPE")
set(TFM_MULTI_CORE_TOPOLOGY             OFF             CACHE BOOL      "Whether to build for a dual-cpu architecture")

################################## Platform specific features ################################
set(USE_ELS_PKC_HUK                    ON          CACHE BOOL      "Use HUK for RW61x platform via els_pkc")
set(USE_ELS_PKC_IAK                    ON          CACHE BOOL      "Use IAK for RW61x platform via els_pkc")
set(USE_HARDENED_PLATFORM_INIT         ON          CACHE BOOL      "Use hardening macros for RW61x platform")
set(MCUX_PSA_CRYPTO_DRIVER_ELS_PKC     ON          CACHE BOOL      "Use psa-crypto-driver to use HW acceleration via driver wrappers for RW61x platform")
set(USE_TFM_EL2GO_DATA_IMPORT_REGION   OFF         CACHE BOOL      "")
set(USE_TFM_WIFI_FLASH_REGION          OFF         CACHE BOOL      "")

############################ Mbedcrypto configurations #########################
set(MBEDCRYPTO_BUILD_TYPE               "${CMAKE_BUILD_TYPE}" CACHE STRING "Build type of Mbed Crypto library")
set(TFM_MBEDCRYPTO_CONFIG_PATH
  "${CMAKE_CURRENT_LIST_DIR}/mbedcrypto_config/tfm_mbedcrypto_config.h" CACHE PATH
  "Config to use for Mbed Crypto. For increased flexibility when pointing to a file, set the type \
of this setting to 'STRING' by passing the :<type> portion when specifying the setting value in \
the command line. E.g. '-DTFM_MBEDCRYPTO_CONFIG_PATH:STRING=some_file_which_is_generated.h' \
This can be useful if the config file is generated and placed inside a directory already added \
to the include path of mbedtls.")
set(TFM_MBEDCRYPTO_PSA_CRYPTO_CONFIG_PATH   "${CMAKE_CURRENT_LIST_DIR}/mbedcrypto_config/crypto_config.h" CACHE PATH "Config to use psa crypto setting for Mbed Crypto.")
set(TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH ""    CACHE PATH      "Config to append to standard Mbed Crypto config, used by platforms to configure feature support")
#set(TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH ${CMAKE_CURRENT_LIST_DIR}/mbedcrypto_config/mbedtls_extra_config.h CACHE PATH "Config to append to standard Mbed Crypto config, used by platforms to cnfigure feature support")

############################ Storage configurations #########################
set(ITS_RAM_FS ON CACHE BOOL "")
set(PS_RAM_FS ON CACHE BOOL "")

################################## Adding Platform Specific Partition ################################
set(TFM_EXTRA_MANIFEST_LIST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/platform/ext/target/nxp/rdrw61x/partitions/tfm_manifest_list.yaml;"  CACHE PATH "Path to extra generated file list. Appended to stardard TFM generated file list." FORCE)
set(TFM_EXTRA_PARTITION_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/platform/ext/target/nxp/rdrw61x/partitions/loader_service;"  CACHE PATH "Path to extra generated file list. Appended to stardard TFM generated file list." FORCE)
set(TFM_PARTITION_LOADER_SERVICE       ON          CACHE BOOL      "Enable secure partition for wifi/ble fw loader service")