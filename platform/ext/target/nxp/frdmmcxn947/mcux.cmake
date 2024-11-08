# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
	
    mcux_add_macro(
        CC "-DTARGET_CONFIG_HEADER_FILE=\\\"config_tfm_target.h\\\"\
        "
    )
    mcux_add_source(
        SOURCES 
        ./common/target_cfg_common.h 
        ./frdmmcxn947/config_tfm_target.h 
        ./frdmmcxn947/target_cfg.c 
        ./frdmmcxn947/target_cfg.h 
        ./frdmmcxn947/tfm_peripherals_def.h 
        ./frdmmcxn947/Device/Include/platform_base_address.h 
        ./frdmmcxn947/partition/flash_layout.h 
        ./frdmmcxn947/partition/region_defs.h
        ./frdmmcxn947/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxn947
    )
    mcux_add_include(
        INCLUDES 
        ./common
		./frdmmcxn947 
        ./frdmmcxn947/Device/Include 
        ./frdmmcxn947/partition
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxn947
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
        ./frdmmcxn947/config_tfm_target.h 
        ./frdmmcxn947/target_cfg.h 
        ./frdmmcxn947/tfm_peripherals_def.h 
        ./frdmmcxn947/Device/Include/platform_base_address.h 
        ./frdmmcxn947/partition/region_defs.h 
        ./frdmmcxn947/partition/flash_layout.h
        ./frdmmcxn947/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxn947
    )
    mcux_add_include(
        INCLUDES 
        ./common 
        ./frdmmcxn947 
        ./frdmmcxn947/partition 
        ./frdmmcxn947/Device/Include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxn947
    )
endif()