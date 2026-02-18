# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./kw47evk/target_cfg.c 
        ./kw47evk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS kw47evk
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./kw47evk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS kw47evk
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
        ./kw47evk/platform_psa_hw_accel.h
        ./kw47evk/config_tfm_target.h 
        ./kw47evk/target_cfg.h 
        ./kw47evk/tfm_peripherals_def.h 
        ./kw47evk/Device/Include/platform_base_address.h 
        ./kw47evk/partition/region_defs.h 
        ./kw47evk/partition/flash_layout.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS kw47evk
    )
    mcux_add_include(
        INCLUDES 
        ./common/
        ./kw47evk/
        ./kw47evk/partition 
        ./kw47evk/Device/Include/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS kw47evk
    )
endif()