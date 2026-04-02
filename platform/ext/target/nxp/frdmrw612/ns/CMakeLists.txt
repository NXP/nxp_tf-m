#-------------------------------------------------------------------------------
# Copyright (c) 2023, Arm Limited. All rights reserved.
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# This file is exported to NS side during CMake installation phase and renamed
# to CMakeLists.txt. It instructs how to build a platform on non-secture side.
# The structure and sources list are fully platform specific.

#========================= Platform Non-Secure ================================#


add_library(platform_ns)


# Startup sources should be put in the executable
target_sources(tfm_ns
    PRIVATE
        Device/Source/startup_frdmrw612.c
)

target_sources(platform_ns
    PRIVATE
        board.c
        hardware_init.c
        clock_config.c
        pin_mux.c
        Driver_USART.c
        flash_config.c
        Native_Driver/system_RW612.c
        Native_Driver/drivers/fsl_usart.c
        Native_Driver/drivers/fsl_cache.c
        Native_Driver/drivers/fsl_clock.c
        Native_Driver/drivers/fsl_common.c
        Native_Driver/drivers/fsl_common_arm.c
        Native_Driver/drivers/fsl_ctimer.c
        Native_Driver/drivers/fsl_flexcomm.c
        Native_Driver/drivers/fsl_flexspi.c
        Native_Driver/drivers/fsl_gpio.c
        Native_Driver/drivers/fsl_ocotp.c
        Native_Driver/drivers/fsl_power.c
        Native_Driver/drivers/fsl_reset.c
        Native_Driver/components/lists/fsl_component_generic_list.c
        Native_Driver/components/serial_manager/fsl_component_serial_manager.c
        Native_Driver/components/serial_manager/fsl_component_serial_port_uart.c
        Native_Driver/components/uart/fsl_adapter_usart.c    
        Native_Driver/utilities/fsl_assert.c
        Native_Driver/utilities/debug_console/fsl_debug_console.c
        Native_Driver/utilities/str/fsl_str.c
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:plat_test.c>

)

target_include_directories(platform_ns
    PUBLIC
        include
        ext/cmsis/Include
        ext/cmsis/Include/m-profile
        ext/common
        ext/driver
        Native_Driver
        Device/Include
        Device/Config
        Native_Driver/drivers
        Native_Driver/components/els_pkc/src/platforms/rw61x
        Native_Driver/components/els_pkc/src/comps/mcuxClEls/inc
        Native_Driver/components/els_pkc/src/comps/mcuxClBuffer/inc
        Native_Driver/components/els_pkc/src/comps/mcuxClBuffer/inc/internal
        Native_Driver/components/els_pkc/src/comps/mcuxClCore/inc
        Native_Driver/components/els_pkc/src/comps/mcuxClMemory/inc
        Native_Driver/components/els_pkc/src/comps/mcuxClMemory/inc/internal
        Native_Driver/components/els_pkc/src/comps/mcuxCsslMemory/inc
        Native_Driver/components/els_pkc/src/comps/mcuxCsslMemory/inc/internal
        Native_Driver/components/els_pkc/src/comps/mcuxCsslParamIntegrity/inc
        Native_Driver/components/els_pkc/src/comps/mcuxCsslFlowProtection/inc
        Native_Driver/components/els_pkc/src/comps/mcuxCsslSecureCounter/inc
        Native_Driver/components/els_pkc/src/comps/mcuxCsslCPreProcessor/inc
        Native_Driver/components/els_pkc/src/compiler
        Native_Driver/components/els_pkc/src/comps/mcuxCsslDataIntegrity/inc
        Native_Driver/components/conn_fwloader/include
        Native_Driver/components/lists
        Native_Driver/components/serial_manager
        Native_Driver/components/uart
        Native_Driver/utilities/debug_console
        Native_Driver/utilities/str
        partition
)

# Include region_defs.h and flash_layout.h
target_include_directories(platform_region_defs
    INTERFACE
        partition
)

target_compile_definitions(platform_ns
    PUBLIC
        ${HARDENING_COMPILE_DEFINITION}
)

target_sources(tfm_api_ns
    PUBLIC
        $<$<BOOL:${TFM_PARTITION_LOADER_SERVICE}>:${INTERFACE_SRC_DIR}/tfm_lds_api.c>
)

if (TEST_NS_ATTESTATION OR TEST_NS_T_COSE)
target_include_directories(platform_ns
    PUBLIC
        initial_attestation
        boot
)
endif()

# Include interface headers exported by TF-M
target_include_directories(tfm_api_ns
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${INTERFACE_INC_DIR}
        Native_Driver/drivers
        Native_Driver/components/conn_fwloader/include
)

