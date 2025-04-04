#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.30.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tfm_s_reg_suites LANGUAGES C CXX ASM 
        PROJECT_BOARD_PORT_PATH middleware/tfm/tf-m/mcux/tfm_s_reg_suites
        CUSTOM_PRJ_CONF_PATH ${SdkRootDirPath}/${board_root}/${board}/lib_example/tfm_s_reg_suites	
		PROJECT_TYPE LIBRARY)

mcux_add_cmakelists(${SdkRootDirPath}/${device_root}/${soc_portfolio}/${soc_series}/${device})

#tem fix, might not work for devices other than RW612
include (${SdkRootDirPath}/drivers/common/CMakeLists.txt OPTIONAL)
include (${SdkRootDirPath}/components/debug_console_lite/CMakeLists.txt OPTIONAL)
include (${SdkRootDirPath}/components/str/CMakeLists.txt OPTIONAL)
include (${SdkRootDirPath}/components/uart/CMakeLists.txt OPTIONAL)
include (${SdkRootDirPath}/middleware/tfm/tf-m/mcux/CMakeLists.txt OPTIONAL)

#add common cc-define for tfm_s_reg_suites lib 
mcux_add_macro(
     CC "-DTFM_SPM_LOG_LEVEL=TFM_SPM_LOG_LEVEL_INFO\
       -DTFM_SP_LOG_RAW_ENABLED\
       -DTFM_PARTITION_LOG_LEVEL=TFM_PARTITION_LOG_LEVEL_INFO\
       -DPLATFORM_NO_FLASH\
       -DDAUTH_CHIP_DEFAULT\
       -D__SEMIHOST_HARDFAULT_DISABLE\
       -DENABLE_HEAP\
       "
)

#include board specific cmake reconfigs
include(${SdkRootDirPath}/examples/_boards/${board}/lib_example/tfm_s_reg_suites/reconfig.cmake OPTIONAL)

#--------------------------
# add iar configurations
#--------------------------
mcux_add_iar_configuration(
    CC "--cmse --diag_suppress Pa039,Pa082,Pa050,Pa205,Pa084,Pe546,Pe550,Pe940,Pe188,Pe186,Pe177,Pe111,Pe174,Pe128,Pe161,Pe068,Pe223,Pa181"
    CX "--cmse --diag_suppress Pa039,Pa082,Pa050,Pa205,Pa084,Pe546,Pe550,Pe940,Pe188,Pe186,Pe177,Pe111,Pe174,Pe128,Pe161,Pe068,Pe223,Pa181"
    LD "--import_cmse_lib_out=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/tfm_s_reg_suites${core_id_suffix_name}.a"
)

#--------------------------
# add armgcc configurations
#--------------------------
mcux_add_armgcc_configuration(
    CC "-mcmse -Wno-unused-variable -Wno-unused-value -Wno-unused-function -Wno-unused-but-set-variable -Wno-return-type -Wno-bool-operation -Wno-maybe-uninitialized -Wno-format"
    CX "-mcmse"
    LD "-Wl,--cmse-implib \
        -Wl,--out-implib=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/tfm_s_reg_suites${core_id_suffix_name}.a"
)

#--------------------------
# add mdk configurations
#--------------------------
mcux_add_mdk_configuration(
    CC "-mcmse"
    CX "-mcmse"
    LD "--diag_suppress=6329,6312,1296 --import-cmse-lib-out=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/tfm_s_reg_suites${core_id_suffix_name}.a"
)
	
# copy to toolchain folder to align ninja build and IDE build
add_custom_command(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${MCUX_SDK_PROJECT_NAME}> ${CMAKE_BINARY_DIR}/${CONFIG_TOOLCHAIN}/tfm_s_reg_suites${core_id_suffix_name}.lib
)

# Create output directory to put library in.
file(MAKE_DIRECTORY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN})