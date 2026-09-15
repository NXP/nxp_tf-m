# Copyright 2024-2025 NXP
# SPDX-License-Identifier: BSD-3-Clause



if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./frdmrw610fml/target_cfg.c 
        ./frdmrw610fml/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmrw610fml
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./frdmrw610fml/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmrw610fml
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
        ./frdmrw610fml/platform_psa_hw_accel.h
        ./frdmrw610fml/config_tfm_target.h 
        ./frdmrw610fml/target_cfg.h 
        ./frdmrw610fml/tfm_peripherals_def.h 
        ./frdmrw610fml/Device/Include/platform_base_address.h 
        ./frdmrw610fml/partition/region_defs.h 
        ./frdmrw610fml/partition/flash_layout.h
        ./rw61x/platform/include/platform_nv_counters_ids.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmrw610fml
    )
    mcux_add_include(
        INCLUDES 
        ./common/
        ./frdmrw610fml/
        ./frdmrw610fml/partition 
        ./frdmrw610fml/Device/Include/
        ./rw61x/platform/include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmrw610fml
    )
endif()
