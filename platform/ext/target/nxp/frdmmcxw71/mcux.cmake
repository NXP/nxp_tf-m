# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./frdmmcxw71/target_cfg.c 
        ./frdmmcxw71/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmmcxw71
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./frdmmcxw71/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmmcxw71
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
        ./frdmmcxw71/platform_psa_hw_accel.h
        ./frdmmcxw71/config_tfm_target.h 
        ./frdmmcxw71/target_cfg.h 
        ./frdmmcxw71/tfm_peripherals_def.h 
        ./frdmmcxw71/Device/Include/platform_base_address.h 
        ./frdmmcxw71/partition/region_defs.h 
        ./frdmmcxw71/partition/flash_layout.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmmcxw71
    )
    mcux_add_include(
        INCLUDES 
        ./common/
        ./frdmmcxw71/
        ./frdmmcxw71/partition 
        ./frdmmcxw71/Device/Include/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmmcxw71
    )
endif()