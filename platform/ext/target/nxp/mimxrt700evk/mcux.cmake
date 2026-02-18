# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause



if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./mimxrt700evk/target_cfg.c 
        ./mimxrt700evk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mimxrt700evk
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./mimxrt700evk/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mimxrt700evk
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
        ./mimxrt700evk/platform_psa_hw_accel.h
        ./mimxrt700evk/config_tfm_target.h 
        ./mimxrt700evk/target_cfg.h 
        ./mimxrt700evk/tfm_peripherals_def.h 
        ./mimxrt700evk/Device/Include/platform_base_address.h 
        ./mimxrt700evk/partition/region_defs.h 
        ./mimxrt700evk/partition/flash_layout.h
		#./rw61x/platform/include/platform_nv_counters_ids.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mimxrt700evk
    )
    mcux_add_include(
        INCLUDES 
		./common/
		./mimxrt700evk/
        ./mimxrt700evk/partition 
		./mimxrt700evk/Device/Include/
		#./rw61x/platform/include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS mimxrt700evk
    )
endif()