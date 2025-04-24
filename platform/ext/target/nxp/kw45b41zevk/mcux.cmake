# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./kw45b41zevk/target_cfg.c 
        ./kw45b41zevk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS kw45b41zevk
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./kw45b41zevk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS kw45b41zevk
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
        ./kw45b41zevk/config_tfm_target.h 
        ./kw45b41zevk/target_cfg.h 
        ./kw45b41zevk/tfm_peripherals_def.h 
        ./kw45b41zevk/Device/Include/platform_base_address.h 
        ./kw45b41zevk/partition/region_defs.h 
        ./kw45b41zevk/partition/flash_layout.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS kw45b41zevk
    )
    mcux_add_include(
        INCLUDES 
        ./common/
        ./kw45b41zevk/
        ./kw45b41zevk/partition 
        ./kw45b41zevk/Device/Include/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS kw45b41zevk
    )
endif()