#-------------------------------------------------------------------------------
# SPDX-FileCopyrightText: Copyright The TrustedFirmware-M Contributors
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

################################## Dependencies ################################
set(TFM_PLATFORM_NXP_HAL_FILE_PATH  "LOCAL"    CACHE STRING     "Path to the NXP SDK hal")
set(TFM_PLATFORM_NXP_HW_ACCEL_SOURCE_PATH  "LOCAL"    CACHE STRING     "Path to the NXP SDK hal")

############################# Platform ##########################################
set(CONFIG_TFM_USE_TRUSTZONE         ON         CACHE BOOL      "Use TrustZone")
set(TFM_MULTI_CORE_TOPOLOGY          OFF        CACHE BOOL      "Platform has multi core")

if(BL2)
############################ BL2 ########################################
set(BL2_S_IMAGE_START               "0x10000"   CACHE STRING    "Base address of the secure image in configuration with BL2")
set(BL2_NS_IMAGE_START              "0x40000"   CACHE STRING    "Base address of the non secure image in configuration with BL2")
endif()

############################ Platform features ##########################################
set(USE_TFM_LPUART_FEATURE           ON         CACHE BOOL      "LPUART enabled")
set(USE_ELS_PKC_HUK                  OFF        CACHE BOOL      "Use HUK via els_pkc")
set(USE_ELS_PKC_IAK                  OFF        CACHE BOOL      "Use IAK via els_pkc")
set(USE_HARDENED_PLATFORM_INIT       ON         CACHE BOOL      "Use hardening macros for frdmmcxa577 platform")
set(TFM_PROFILE               "profile_medium"  CACHE STRING "" FORCE)

########################### HW ACCELERATION #############################################

# HW acceleration related configs managed below
if(CRYPTO_HW_ACCELERATOR)
set(CRYPTO_HW_ACCELERATOR_TYPE      "nxp"       CACHE STRING    "The hardware accelerator platform")
set(CONFIG_PSA_CRYPTO_DRIVER_SGI_PKC ON         CACHE BOOL      "Use psa-crypto-driver to use HW acceleration via driver wrappers")
set(CONFIG_SGI_PKC                   ON         CACHE BOOL      "HW accelration ON")
set(CONFIG_SGI_PKC_PLATFORM_MCXA     ON         CACHE BOOL      "HW accelration ON for MCXA Platform")
set(CONFIG_BUILD_WITH_TFM            ON         CACHE BOOL      "Introduced configuration flag to be used in HW accel libs cmake")
endif()

################################## LOG LEVEL #############################################################
set(TFM_SPM_LOG_LEVEL          LOG_LEVEL_INFO   CACHE STRING     "Set default SPM log level as INFO level" FORCE)
set(TFM_PARTITION_LOG_LEVEL    LOG_LEVEL_INFO   CACHE STRING     "Set default Secure Partition log level as INFO level" FORCE)
set(CONFIG_TFM_INCLUDE_STDLIBC       ON         CACHE BOOL      "Include standard C libraries and startup code")