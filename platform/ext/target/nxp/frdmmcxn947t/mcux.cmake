# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES 
        ./frdmmcxn947t/target_cfg.c 
        ./frdmmcxn947t/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxn947t
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES 
        ./frdmmcxn947t/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxn947t
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
        ./frdmmcxn947t/platform_psa_hw_accel.h
        ./frdmmcxn947t/config_tfm_target.h 
        ./frdmmcxn947t/target_cfg.h 
        ./frdmmcxn947t/tfm_peripherals_def.h 
        ./frdmmcxn947t/Device/Include/platform_base_address.h 
        ./frdmmcxn947t/partition/region_defs.h 
        ./frdmmcxn947t/partition/flash_layout.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxn947t
    )
    mcux_add_include(
        INCLUDES 
		./common/
		./frdmmcxn947t/
        ./frdmmcxn947t/partition 
		./frdmmcxn947t/Device/Include/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
		BOARDS frdmmcxn947t
    )
endif()