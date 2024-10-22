#-------------------------------------------------------------------------------
# Copyright (c) 2021-2023, Arm Limited. All rights reserved.
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_LOADER_SERVICE)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_lds STATIC)

target_include_directories(tfm_psa_rot_partition_lds
    INTERFACE
        ${CMAKE_SOURCE_DIR}/secure_fw/include
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/loader_service
)
target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/loader_service
)

target_sources(tfm_psa_rot_partition_lds
    PRIVATE
        tfm_lds.c
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/loader_service/auto_generated/intermedia_tfm_loader_service.c
)
target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/loader_service/auto_generated/load_info_tfm_loader_service.c
)

target_link_libraries(tfm_psa_rot_partition_lds
    PRIVATE
        platform_s
        tfm_config
        tfm_sprt
)

# Generate loader service feature
if (NOT LDS_DEVICE_CONFIG_FILE)
    configure_file(${CMAKE_SOURCE_DIR}/interface/include/psa/lds_config.h.in
                   ${CMAKE_BINARY_DIR}/generated/interface/include/psa/config_lds.h
                   @ONLY)
    set(LDS_DEVICE_CONFIG_FILE "${CMAKE_BINARY_DIR}/generated/interface/include/psa/config_lds.h")
else()
    # LDS_DEVICE_CONFIG_FILE exists and is a file
    if(NOT EXISTS ${LDS_DEVICE_CONFIG_FILE})
        message(FATAL_ERROR "LDS_DEVICE_CONFIG_FILE:${LDS_DEVICE_CONFIG_FILE} does not exist.")
    elseif(IS_DIRECTORY ${LDS_DEVICE_CONFIG_FILE})
        message(FATAL_ERROR "LDS_DEVICE_CONFIG_FILE:${LDS_DEVICE_CONFIG_FILE} is a folder while a file is expected.")
    endif()
endif()

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_lds
)

target_compile_definitions(tfm_config
    INTERFACE
    TFM_PARTITION_LOADER_SERVICE
)
