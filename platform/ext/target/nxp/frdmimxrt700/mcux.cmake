# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause



if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./frdmimxrt700/target_cfg.c 
        ./frdmimxrt700/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmimxrt700
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./frdmimxrt700/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmimxrt700
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
        ./frdmimxrt700/platform_psa_hw_accel.h
        ./frdmimxrt700/config_tfm_target.h 
        ./frdmimxrt700/target_cfg.h 
        ./frdmimxrt700/tfm_peripherals_def.h 
        ./frdmimxrt700/Device/Include/platform_base_address.h 
        ./frdmimxrt700/partition/region_defs.h 
        ./frdmimxrt700/partition/flash_layout.h
		#./rw61x/platform/include/platform_nv_counters_ids.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmimxrt700
    )
    mcux_add_include(
        INCLUDES 
		./common/
		./frdmimxrt700/
        ./frdmimxrt700/partition 
		./frdmimxrt700/Device/Include/
		#./rw61x/platform/include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmimxrt700
    )
endif()