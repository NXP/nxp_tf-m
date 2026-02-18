# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./frdmmcxw72/target_cfg.c 
        ./frdmmcxw72/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmmcxw72
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./frdmmcxw72/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmmcxw72
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.board_headers)
    mcux_add_macro(
        CC "-DTARGET_CONFIG_HEADER_FILE=\\\"config_tfm_target.h\\\"\
        "
    )
    mcux_add_source(
        SOURCES
        ./common/target_cfg_common.h 
        ./frdmmcxw72/platform_psa_hw_accel.h
        ./frdmmcxw72/config_tfm_target.h 
        ./frdmmcxw72/target_cfg.h 
        ./frdmmcxw72/tfm_peripherals_def.h 
        ./frdmmcxw72/Device/Include/platform_base_address.h 
        ./frdmmcxw72/partition/region_defs.h 
        ./frdmmcxw72/partition/flash_layout.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmmcxw72
    )
    mcux_add_include(
        INCLUDES 
        ./common/
        ./frdmmcxw72/
        ./frdmmcxw72/partition 
        ./frdmmcxw72/Device/Include/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmmcxw72
    )
endif()