# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./mcxn9xxevk/target_cfg.c 
        ./mcxn9xxevk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mcxn9xxevk
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./mcxn9xxevk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mcxn9xxevk
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
        ./mcxn9xxevk/config_tfm_target.h 
        ./mcxn9xxevk/target_cfg.h 
        ./mcxn9xxevk/tfm_peripherals_def.h 
        ./mcxn9xxevk/Device/Include/platform_base_address.h 
        ./mcxn9xxevk/partition/region_defs.h 
        ./mcxn9xxevk/partition/flash_layout.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mcxn9xxevk
    )
    mcux_add_include(
        INCLUDES 
		./common/
		./mcxn9xxevk/
        ./mcxn9xxevk/partition 
		./mcxn9xxevk/Device/Include/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mcxn9xxevk
    )
endif()