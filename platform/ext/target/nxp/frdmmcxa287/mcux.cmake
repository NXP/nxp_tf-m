# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.board)
    mcux_add_source(
        SOURCES
        ./frdmmcxa287/target_cfg.c
        ./frdmmcxa287/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmmcxa287
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.board)
    mcux_add_source(
        SOURCES
        ./frdmmcxa287/Device/Source/startup_${board}.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmmcxa287
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
        ./frdmmcxa287/platform_psa_hw_accel.h
        ./frdmmcxa287/config_tfm_target.h
        ./frdmmcxa287/target_cfg.h
        ./frdmmcxa287/tfm_peripherals_def.h
        ./frdmmcxa287/Device/Include/platform_base_address.h
        ./frdmmcxa287/partition/region_defs.h
        ./frdmmcxa287/partition/flash_layout.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmmcxa287
    )
    mcux_add_include(
        INCLUDES
        ./common/
        ./frdmmcxa287/
        ./frdmmcxa287/partition
        ./frdmmcxa287/Device/Include/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp
        BOARDS frdmmcxa287
    )
endif()