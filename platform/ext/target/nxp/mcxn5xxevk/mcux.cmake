# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./mcxn5xxevk/target_cfg.c 
        ./mcxn5xxevk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mcxn5xxevk
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./mcxn5xxevk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mcxn5xxevk
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
        ./mcxn5xxevk/platform_psa_hw_accel.h
        ./mcxn5xxevk/config_tfm_target.h 
        ./mcxn5xxevk/target_cfg.h 
        ./mcxn5xxevk/tfm_peripherals_def.h 
        ./mcxn5xxevk/Device/Include/platform_base_address.h 
        ./mcxn5xxevk/partition/region_defs.h 
        ./mcxn5xxevk/partition/flash_layout.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mcxn5xxevk
    )
    mcux_add_include(
        INCLUDES 
		./common/
		./mcxn5xxevk/
        ./mcxn5xxevk/partition 
		./mcxn5xxevk/Device/Include/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mcxn5xxevk
    )
endif()
