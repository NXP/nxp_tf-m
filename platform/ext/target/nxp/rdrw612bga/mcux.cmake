# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause



if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./rdrw612bga/target_cfg.c 
        ./rdrw612bga/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS rdrw612bga
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./rdrw612bga/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS rdrw612bga
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
        ./rdrw612bga/platform_psa_hw_accel.h
        ./rdrw612bga/config_tfm_target.h 
        ./rdrw612bga/target_cfg.h 
        ./rdrw612bga/tfm_peripherals_def.h 
        ./rdrw612bga/Device/Include/platform_base_address.h 
        ./rdrw612bga/partition/region_defs.h 
        ./rdrw612bga/partition/flash_layout.h
		./rw61x/platform/include/platform_nv_counters_ids.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS rdrw612bga
    )
    mcux_add_include(
        INCLUDES 
		./common/
		./rdrw612bga/
        ./rdrw612bga/partition 
		./rdrw612bga/Device/Include/
		./rw61x/platform/include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS rdrw612bga
    )
endif()