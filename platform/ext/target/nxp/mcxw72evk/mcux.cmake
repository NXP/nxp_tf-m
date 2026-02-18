# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./mcxw72evk/target_cfg.c 
        ./mcxw72evk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS mcxw72evk
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./mcxw72evk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS mcxw72evk
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
        ./mcxw72evk/platform_psa_hw_accel.h
        ./mcxw72evk/config_tfm_target.h 
        ./mcxw72evk/target_cfg.h 
        ./mcxw72evk/tfm_peripherals_def.h 
        ./mcxw72evk/Device/Include/platform_base_address.h 
        ./mcxw72evk/partition/region_defs.h 
        ./mcxw72evk/partition/flash_layout.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS mcxw72evk
    )
    mcux_add_include(
        INCLUDES 
        ./common/
        ./mcxw72evk/
        ./mcxw72evk/partition 
        ./mcxw72evk/Device/Include/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS mcxw72evk
    )
endif()