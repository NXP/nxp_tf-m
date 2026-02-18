# Copyright 2024-2025 NXP
# SPDX-License-Identifier: BSD-3-Clause



if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./frdmrw612/target_cfg.c 
        ./frdmrw612/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmrw612
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./frdmrw612/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmrw612
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
        ./frdmrw612/platform_psa_hw_accel.h
        ./frdmrw612/config_tfm_target.h 
        ./frdmrw612/target_cfg.h 
        ./frdmrw612/tfm_peripherals_def.h 
        ./frdmrw612/Device/Include/platform_base_address.h 
        ./frdmrw612/partition/region_defs.h 
        ./frdmrw612/partition/flash_layout.h
		./rw61x/platform/include/platform_nv_counters_ids.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmrw612
    )
    mcux_add_include(
        INCLUDES 
		./common/
		./frdmrw612/
        ./frdmrw612/partition 
		./frdmrw612/Device/Include/
		./rw61x/platform/include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmrw612
    )
endif()