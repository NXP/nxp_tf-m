#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
# Copyright 2023-2024 NXP
# Copyright (c) 2020, Linaro. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)

set(NXP_COMMON_DIR ${CMAKE_CURRENT_LIST_DIR}/../common)

# Pull drivers from NXP git
if (TFM_PLATFORM_NXP_HAL_FILE_PATH STREQUAL "DOWNLOAD")
    set(NXP_HAL_FILE_PATH ${PLATFORM_DIR}/ext/target/nxp)
    Message("-- Pulling MCUxpresso NXP SDK drivers from https://github.com/NXPmicro/mcux-sdk")
    include(pull_drivers.cmake)
# Use specified path to NXP hal folder provided by user
else()
    message("Using nxp mcux source code from ${TFM_PLATFORM_NXP_HAL_FILE_PATH}.")
    set(NXP_HAL_FILE_PATH ${TFM_PLATFORM_NXP_HAL_FILE_PATH})
endif()

# configuration options for library builds of els_pkc and psa_crypto_driver
set(CONFIG_BUILD_WITH_TFM ON)
set(CONFIG_SOC_SERIES_RW6XX ON)
set(CONFIG_EXPORT_MCUX_ELS_PKC_TARGETS ON)

#add_t
set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_LOWER)
if (BL2 AND ${BUILD_TYPE_LOWER} STREQUAL "debug")
    Message(FATAL_ERROR "If BL2 is enabled, \"${CMAKE_BUILD_TYPE}\" build type cannot be selected as it is currently not \
                         supported on ${TFM_PLATFORM}. For more information please refer to the platform's README file.")
endif()

set(HARDENING_COMPILE_DEFINITION "HARDENING_MACROS_ENABLED")
if (DEFINED USE_HARDENED_PLATFORM_INIT)
    if(NOT ${USE_HARDENED_PLATFORM_INIT})
        set(HARDENING_COMPILE_DEFINITION "")
    endif()
endif()

if (USE_TFM_EL2GO_DATA_IMPORT_REGION)
    set(TFM_EL2GO_DATA_IMPORT_REGION_COMPILE_DEFINITION "TFM_EL2GO_DATA_IMPORT_REGION")
endif()

if (USE_TFM_WIFI_FLASH_REGION)
    set(TFM_WIFI_FLASH_REGION_COMPILE_DEFINITION "TFM_WIFI_FLASH_REGION")
endif()

#========================= Platform region defs ===============================#

target_include_directories(platform_region_defs
    INTERFACE
        partition
)

#========================= Platform common defs ===============================#
target_compile_definitions(psa_interface
    INTERFACE
        ${TFM_EL2GO_DATA_IMPORT_REGION_COMPILE_DEFINITION}
        ${TFM_WIFI_FLASH_REGION_COMPILE_DEFINITION}
)

target_compile_definitions(tfm_s
    PUBLIC
        BOOT_HEADER_ENABLE=1
    )

# Specify the location of platform specific build dependencies.
target_sources(tfm_s
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/startup_rdrw61x.c
)

# Provide with the flash config files for rw61x
target_sources(tfm_s
    PUBLIC
        ${NXP_HAL_FILE_PATH}/rdrw61x/project_template/flash_config/flash_config.c
)

target_include_directories(tfm_s
    PUBLIC
        ${NXP_HAL_FILE_PATH}/rdrw61x/project_template/flash_config
)

target_add_scatter_file(tfm_s
    $<$<C_COMPILER_ID:ARMClang>:${PLATFORM_DIR}/platform/ext/target/nxp/common/armclang/tfm_common_s.sct>
    $<$<C_COMPILER_ID:GNU>:${CMAKE_SOURCE_DIR}/platform/ext/target/nxp/common/gcc/tfm_common_s.ld>
    $<$<C_COMPILER_ID:IAR>:${CMAKE_SOURCE_DIR}/platform/ext/target/nxp/common/iar/tfm_common_s.icf>
)



#========================= Platform Secure ====================================#

target_compile_definitions(platform_s
    PUBLIC
        FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1
        ${HARDENING_COMPILE_DEFINITION}
        CPU_RW612ETA2I
        CPU_RW612ETA1I
)

target_include_directories(platform_s
    PUBLIC
        .
        Device/Include
        project_template/s
        partition
        ../../arm/drivers/mpu/armv8m
        ${PLATFORM_DIR}/ext/target/nxp/common
        ${PLATFORM_DIR}/ext/target/nxp/common/CMSIS_Driver
        ${PLATFORM_DIR}/ext/target/nxp/common/Device/Config
        ${PLATFORM_DIR}/ext/target/nxp/common/Device/Include
        ${NXP_HAL_FILE_PATH}/common/Native_Driver
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers
        ${NXP_HAL_FILE_PATH}/rdrw61x/CMSIS_Driver
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers

        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/romapi
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/romapi
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/romapi/bootloader
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/romapi/flexspi
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/romapi/iap
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/romapi/nboot
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/romapi/otp

        ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/lists
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/serial_manager
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/uart
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/debug_console
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/str
)

target_sources(platform_s
    PRIVATE
        ${NXP_COMMON_DIR}/CMSIS_Driver/Driver_USART.c
        ../../arm/drivers/mpu/armv8m/mpu_armv8m_drv.c
        project_template/s/board.c
        project_template/s/clock_config.c
        project_template/s/pin_mux.c

        ${PLATFORM_DIR}/ext/target/nxp/rdrw61x/CMSIS_Driver/Driver_Flash_iap_rw61x.c
        ${PLATFORM_DIR}/ext/target/nxp/rdrw61x/CMSIS_Driver/Driver_Flash_iap_rw61x_iped.c

        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/system_RW612.c
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/fsl_clock.c
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/fsl_iped.c
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/fsl_power.c
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/fsl_reset.c
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/fsl_ocotp.c
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/romapi/bootloader/fsl_romapi.c
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/romapi/flexspi/fsl_romapi_flexspi.c
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/romapi/iap/fsl_romapi_iap.c
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/romapi/nboot/fsl_romapi_nboot.c
        ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/romapi/otp/fsl_romapi_otp.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/lists/fsl_component_generic_list.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/serial_manager/fsl_component_serial_manager.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/serial_manager/fsl_component_serial_port_uart.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/uart/fsl_adapter_usart.c

        ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_common.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_common_arm.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_ctimer.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_flexcomm.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_gpio.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_cache.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_flexspi.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_trng.c

        ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_usart.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/fsl_assert.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/debug_console/fsl_debug_console.c
        ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/str/fsl_str.c

        $<$<NOT:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>>:${PLATFORM_DIR}/ext/target/nxp/rdrw61x/nv_counters.c>

        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${PLATFORM_DIR}/ext/target/nxp/common/plat_test.c>
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${PLATFORM_DIR}/ext/target/nxp/common/services/src/tfm_platform_system.c>
        # ${NXP_HAL_FILE_PATH}/common/plat_attest_hal.c
)

#add_subdirectory(${NXP_HAL_FILE_PATH}/common/Native_Driver/components/els_pkc els_pkc)
add_subdirectory(${MBEDCRYPTO_PATH}/../els_pkc els_pkc)

target_link_libraries(platform_s
    PRIVATE
    # For attest HAL strnlen
        tfm_sprt
        McuxElsPkc
)

if (MCUX_PSA_CRYPTO_DRIVER_ELS_PKC)
    add_subdirectory(${NXP_HAL_FILE_PATH}/common/Native_Driver/components/psa_crypto_driver psa_crypto_driver)
    target_link_libraries(platform_s
        PRIVATE
            McuxPsaCryptoDriverElsPkc
    )
endif()

target_compile_options(platform_s
    PUBLIC
        ${COMPILER_CMSE_FLAG}
)

# GNU ld has some strange behaviour to do with weak functions, and does not deal
# well with strong functions in static libraries overriding weak functions in
# object files. For this reason, the file hardware_init is linked directly to
# the s binary as an object file. This issue does not affect armclang, but the
# fix is compatible.
target_sources(tfm_s
    PRIVATE
        project_template/s/hardware_init.c
)

#========================= Platform customizations ============================================#

if (TFM_PARTITION_CRYPTO)

    target_compile_options(platform_crypto_keys
        PUBLIC
            ${COMPILER_CMSE_FLAG}
    )

    target_sources(platform_crypto_keys
        PRIVATE
            crypto_keys.c
    )

    target_link_libraries(platform_crypto_keys
        PRIVATE
            cmsis
    )

    target_include_directories(platform_crypto_keys
        PUBLIC
            ./Device/Include
            ${PLATFORM_DIR}/ext/target/nxp/common/Device/Config
            ${PLATFORM_DIR}/ext/target/nxp/common/Device/Include
            ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver
            # platform_crypto_keys does not link against els_pkc and PsaCryptoDriverElsPkc, but we need to include it's
            # header files to have the function declarations, typedefs and defines available.
            # Those directories are declared in the included library cmake file and can be
            # used here:
            "$<TARGET_PROPERTY:McuxElsPkc,INTERFACE_INCLUDE_DIRECTORIES>"
    )

    target_compile_definitions(platform_crypto_keys
        PUBLIC
            $<$<BOOL:${USE_ELS_PKC_HUK}>:USE_ELS_PKC_HUK>
            $<$<BOOL:${USE_ELS_PKC_IAK}>:USE_ELS_PKC_IAK>
    )
if (MCUX_PSA_CRYPTO_DRIVER_ELS_PKC)

    target_include_directories(platform_crypto_keys
        PUBLIC
            "$<TARGET_PROPERTY:McuxPsaCryptoDriverElsPkc,INTERFACE_INCLUDE_DIRECTORIES>"
    )
    target_include_directories(tfm_psa_rot_partition_crypto
        PUBLIC
            # tfm_psa_rot_partition_crypto does not link against PsaCryptoDriverElsPkc directly, but we need to include it's
            # header files to have the function declarations, typedefs and defines available.
            # Those directories are declared in the included library cmake file and can be used here:
            "$<TARGET_PROPERTY:McuxPsaCryptoDriverElsPkc,INTERFACE_INCLUDE_DIRECTORIES>"
    )

    target_include_directories(crypto_service_mbedcrypto
        PUBLIC
            # crypto_service_mbedcrypto does not link against PsaCryptoDriverElsPkc directly, but we need to include it's
            # header files to have the function declarations, typedefs and defines available.
            # Those directories are declared in the included library cmake file and can be used here:
            "$<TARGET_PROPERTY:McuxPsaCryptoDriverElsPkc,INTERFACE_INCLUDE_DIRECTORIES>"
    )
    target_compile_definitions(crypto_service_mbedcrypto
        PUBLIC
            $<$<BOOL:${MCUX_PSA_CRYPTO_DRIVER_ELS_PKC}>:PSA_CRYPTO_DRIVER_ELS_PKC>
    )
endif()
endif()
#========================= tfm_spm ============================================#

target_sources(tfm_spm
    PRIVATE
        target_cfg.c
        ${NXP_COMMON_DIR}/tfm_hal_isolation.c
        ${NXP_COMMON_DIR}/tfm_hal_platform.c
)

# If this is not added to the tfm_s it will not correctly override the weak
# default handlers declared in assemebly, and will instead be discarded as they
# are not in use.
target_sources(tfm_s
    PRIVATE
        ${NXP_COMMON_DIR}/mpc_ppc_faults.c
)

#========================= Install platform specific interface files ===============================#
install(FILES
    ${NXP_HAL_FILE_PATH}/rdrw61x/Device/Include/tfm_builtin_key_ids.h
    DESTINATION "${INSTALL_PLATFORM_NS_DIR}/../interface/include"
)

#========================= Files for building NS side platform ================#
install(FILES       ${NXP_HAL_FILE_PATH}/rdrw61x/project_template/ns/board.c
                    ${NXP_HAL_FILE_PATH}/rdrw61x/project_template/ns/hardware_init.c
                    ${NXP_HAL_FILE_PATH}/rdrw61x/project_template/ns/clock_config.c
                    ${NXP_HAL_FILE_PATH}/rdrw61x/project_template/ns/pin_mux.c
                    ${NXP_HAL_FILE_PATH}/rdrw61x/project_template/flash_config/flash_config.c
                    ${NXP_COMMON_DIR}/CMSIS_Driver/Driver_USART.c
                    ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/system_RW612.c
                    ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/fsl_clock.c
                    ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/fsl_power.c
                    ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver/drivers/fsl_reset.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/lists/fsl_component_generic_list.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/serial_manager/fsl_component_serial_manager.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/serial_manager/fsl_component_serial_port_uart.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/uart/fsl_adapter_usart.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_common.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_common_arm.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_ctimer.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_flexcomm.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_gpio.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_usart.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/fsl_assert.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/debug_console/fsl_debug_console.c
                    ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/str/fsl_str.c
                    $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${PLATFORM_DIR}/ext/target/nxp/common/plat_test.c>
        DESTINATION ${INSTALL_PLATFORM_NS_DIR})

install(DIRECTORY   ${NXP_COMMON_DIR}/Device
        DESTINATION ${INSTALL_PLATFORM_NS_DIR})

install(DIRECTORY   ${TARGET_PLATFORM_PATH}/Device
        DESTINATION ${INSTALL_PLATFORM_NS_DIR})

install(DIRECTORY   ${NXP_HAL_FILE_PATH}/rdrw61x/Native_Driver
        DESTINATION ${INSTALL_PLATFORM_NS_DIR})

#install(DIRECTORY   ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/els_pkc
#        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/Native_Driver/components)

install(DIRECTORY   ${MBEDCRYPTO_PATH}/../els_pkc/src
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/Native_Driver/components/els_pkc)

install(DIRECTORY   ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/lists
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/Native_Driver/components)

install(DIRECTORY   ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/serial_manager
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/Native_Driver/components)

install(DIRECTORY   ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/uart
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/Native_Driver/components)

install(DIRECTORY   ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/Native_Driver)

install(DIRECTORY   ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/Native_Driver)

install(FILES       ${PLATFORM_DIR}/ext/driver/Driver_Common.h
                    ${PLATFORM_DIR}/ext/driver/Driver_Flash.h
                    ${PLATFORM_DIR}/ext/driver/Driver_USART.h
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ext/driver)

install(FILES       ${TARGET_PLATFORM_PATH}/project_template/ns/app.h
                    ${TARGET_PLATFORM_PATH}/project_template/ns/board.h
                    ${TARGET_PLATFORM_PATH}/project_template/ns/clock_config.h
                    ${TARGET_PLATFORM_PATH}/project_template/ns/pin_mux.h
                    ${TARGET_PLATFORM_PATH}/project_template/flash_config/flash_config.h
                    ${TARGET_PLATFORM_PATH}/config_tfm_target.h
                    ${TARGET_PLATFORM_PATH}/target_cfg.h
                    ${TARGET_PLATFORM_PATH}/tfm_peripherals_def.h
                    ${NXP_COMMON_DIR}/target_cfg_common.h

        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/include)

install(DIRECTORY   ${TARGET_PLATFORM_PATH}/partition
        DESTINATION ${INSTALL_PLATFORM_NS_DIR})

install(FILES       ${PLATFORM_DIR}/ext/target/nxp/common/gcc/tfm_common_ns.ld
                    ${PLATFORM_DIR}/ext/target/nxp/common/armclang/tfm_common_ns.sct
                    ${PLATFORM_DIR}/ext/target/nxp/common/iar/tfm_common_ns.icf
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/linker_scripts)

# copy all files from active platform directory
install(DIRECTORY   ${TARGET_PLATFORM_PATH}/ns/
        DESTINATION ${INSTALL_PLATFORM_NS_DIR})

install(FILES       ${TARGET_PLATFORM_PATH}/cpuarch.cmake
                    ${TARGET_PLATFORM_PATH}/config.cmake
        DESTINATION ${INSTALL_PLATFORM_NS_DIR})

if (TEST_NS_ATTESTATION OR TEST_NS_T_COSE)
install(DIRECTORY   ${NXP_HAL_FILE_PATH}/../../../../lib/ext/t_cose
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/../)

install(DIRECTORY   ${NXP_HAL_FILE_PATH}/../../../../secure_fw/spm/include/boot
        DESTINATION ${INSTALL_PLATFORM_NS_DIR})

install(FILES       ${NXP_HAL_FILE_PATH}/../../../../secure_fw/partitions/initial_attestation/attest.h
                    ${NXP_HAL_FILE_PATH}/../../../../secure_fw/partitions/initial_attestation/attest_key.h
                    ${NXP_HAL_FILE_PATH}/../../../../secure_fw/partitions/initial_attestation/attest_token.h
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/initial_attestation)
endif()
