# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./mcxw23evk/target_cfg.c 
        ./mcxw23evk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mcxw23evk
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./mcxw23evk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mcxw23evk
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
        ./mcxw23evk/platform_psa_hw_accel.h
        ./mcxw23evk/config_tfm_target.h 
        ./mcxw23evk/target_cfg.h 
        ./mcxw23evk/tfm_peripherals_def.h 
        ./mcxw23evk/Device/Include/platform_base_address.h 
        ./mcxw23evk/partition/region_defs.h 
        ./mcxw23evk/partition/flash_layout.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mcxw23evk
    )
    mcux_add_include(
        INCLUDES 
		./common/
		./mcxw23evk/
        ./mcxw23evk/partition 
		./mcxw23evk/Device/Include/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mcxw23evk
    )
endif()