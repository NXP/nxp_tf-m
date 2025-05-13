# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./frdmmcxw23/target_cfg.c 
        ./frdmmcxw23/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxw23
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./frdmmcxw23/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxw23
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
        ./frdmmcxw23/config_tfm_target.h 
        ./frdmmcxw23/target_cfg.h 
        ./frdmmcxw23/tfm_peripherals_def.h 
        ./frdmmcxw23/Device/Include/platform_base_address.h 
        ./frdmmcxw23/partition/region_defs.h 
        ./frdmmcxw23/partition/flash_layout.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxw23
    )
    mcux_add_include(
        INCLUDES 
		./common/
		./frdmmcxw23/
        ./frdmmcxw23/partition 
		./frdmmcxw23/Device/Include/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxw23
    )
endif()