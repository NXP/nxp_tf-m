# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
	
    mcux_add_macro(
        CC "-DTARGET_CONFIG_HEADER_FILE=\\\"config_tfm_target.h\\\"\
        "
    )
    mcux_add_source(
        SOURCES 
        ./common/target_cfg_common.h 
        ./frdmmcxn236/config_tfm_target.h 
        ./frdmmcxn236/target_cfg.c 
        ./frdmmcxn236/target_cfg.h 
        ./frdmmcxn236/tfm_peripherals_def.h 
        ./frdmmcxn236/Device/Include/platform_base_address.h 
        ./frdmmcxn236/partition/flash_layout.h 
        ./frdmmcxn236/partition/region_defs.h
        ./frdmmcxn236/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxn236
    )
    mcux_add_include(
        INCLUDES 
        ./common
		./frdmmcxn236 
        ./frdmmcxn236/Device/Include 
        ./frdmmcxn236/partition
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxn236
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)

    mcux_add_macro(
         CC "-DTARGET_CONFIG_HEADER_FILE=\\\"config_tfm_target.h\\\"\
            "
    )
    mcux_add_source(
        SOURCES 
        ./common/target_cfg_common.h 
        ./frdmmcxn236/config_tfm_target.h 
        ./frdmmcxn236/target_cfg.h 
        ./frdmmcxn236/tfm_peripherals_def.h 
        ./frdmmcxn236/Device/Include/platform_base_address.h 
        ./frdmmcxn236/partition/region_defs.h 
        ./frdmmcxn236/partition/flash_layout.h
        ./frdmmcxn236/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxn236
    )
    mcux_add_include(
        INCLUDES 
        ./common 
        ./frdmmcxn236 
        ./frdmmcxn236/partition 
        ./frdmmcxn236/Device/Include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxn236
    )
endif()