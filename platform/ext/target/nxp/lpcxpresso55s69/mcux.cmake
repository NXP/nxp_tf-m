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
        ./lpcxpresso55s69/config_tfm_target.h 
        ./lpcxpresso55s69/target_cfg.c 
        ./lpcxpresso55s69/target_cfg.h 
        ./lpcxpresso55s69/tfm_peripherals_def.h 
        ./lpcxpresso55s69/Device/Include/platform_base_address.h 
        ./lpcxpresso55s69/partition/flash_layout.h 
        ./lpcxpresso55s69/partition/region_defs.h
        ./lpcxpresso55s69/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS lpcxpresso55s69
    )
    mcux_add_include(
        INCLUDES 
        ./common
		./lpcxpresso55s69 
        ./lpcxpresso55s69/Device/Include 
        ./lpcxpresso55s69/partition
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS lpcxpresso55s69
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
        ./lpcxpresso55s69/platform_psa_hw_accel.h
        ./lpcxpresso55s69/config_tfm_target.h 
        ./lpcxpresso55s69/target_cfg.h 
        ./lpcxpresso55s69/tfm_peripherals_def.h 
        ./lpcxpresso55s69/Device/Include/platform_base_address.h 
        ./lpcxpresso55s69/partition/region_defs.h 
        ./lpcxpresso55s69/partition/flash_layout.h
        ./lpcxpresso55s69/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS lpcxpresso55s69
    )
    mcux_add_include(
        INCLUDES 
        ./common 
        ./lpcxpresso55s69 
        ./lpcxpresso55s69/partition 
        ./lpcxpresso55s69/Device/Include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS lpcxpresso55s69
    )
endif()