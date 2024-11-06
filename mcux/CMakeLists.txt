#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s)
    mcux_add_macro(
            CC "-DCONFIG_TFM_BUILDING_SPE=1\
            -DCONFIG_TFM_ENABLE_MEMORY_PROTECT\
            -DTFM_PARTITION_NS_AGENT_TZ\
            -DTFM_PARTITION_IDLE\
            -DPLATFORM_DEFAULT_OTP\
            -DPLATFORM_DEFAULT_PROVISIONING\
            -DOTP_WRITEABLE\
            -DTFM_DUMMY_PROVISIONING\
            -DCONFIG_TFM_PARTITION_META\
            -DCONFIG_TFM_FLOAT_ABI=2\
            -DCONFIG_TFM_ENABLE_CP10CP11\
            -DCONFIG_TFM_LAZY_STACKING\
            -DCONFIG_TFM_HALT_ON_CORE_PANIC\
            -DCONFIG_TFM_USE_TRUSTZONE\
            -DATTEST_TOKEN_PROFILE_PSA_IOT_1\
            -DTEST_NS_ATTESTATION\
            -DTEST_S_ATTESTATION\
            -DPS_ENCRYPTION\
            -DPS_ROLLBACK_PROTECTION\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/lib/fih/inc/fih.h
        tf-m/lib/fih/inc/tfm_fih_rng.h
        tf-m/lib/fih/src/fih.c
        tf-m/platform/ext/common/common_target_cfg.h
        tf-m/platform/ext/common/core_ext.h
        tf-m/platform/ext/common/exception_info.c
        tf-m/platform/ext/common/faults.c
        tf-m/platform/ext/common/provisioning.c
        tf-m/platform/ext/common/tfm_hal_its.c
        tf-m/platform/ext/common/tfm_hal_nvic.c
        tf-m/platform/ext/common/tfm_hal_ps.c
        tf-m/platform/ext/common/tfm_hal_sp_logdev.h
        tf-m/platform/ext/common/tfm_hal_sp_logdev_periph.c
        tf-m/platform/ext/common/tfm_hal_spm_logdev.h
        tf-m/platform/ext/common/tfm_hal_spm_logdev_peripheral.c
        tf-m/platform/ext/common/tfm_hal_reset_halt.c
        tf-m/platform/ext/common/tfm_interrupts.c
        tf-m/platform/ext/common/tfm_s_linker_alignments.h
        tf-m/platform/ext/common/uart_stdout.c
        tf-m/platform/ext/common/uart_stdout.h
        tf-m/platform/ext/driver/Driver_Common.h
        tf-m/platform/ext/driver/Driver_Flash.h
        tf-m/platform/ext/driver/Driver_MPC.h
        tf-m/platform/ext/driver/Driver_PPC.h
        tf-m/platform/ext/driver/Driver_USART.h
        tf-m/platform/ext/common/template/attest_hal.c
        tf-m/platform/ext/common/template/flash_otp_nv_counters_backend.c
        tf-m/platform/ext/common/template/flash_otp_nv_counters_backend.h
        tf-m/platform/ext/common/template/otp_flash.c
        tf-m/platform/ext/common/template/tfm_fih_rng.c
        tf-m/platform/ext/accelerator/interface/crypto_hw.h
        tf-m/platform/ext/target/arm/drivers/mpu/armv8m/mpu_armv8m_drv.c
        tf-m/platform/ext/target/arm/drivers/mpu/armv8m/mpu_armv8m_drv.h
        tf-m/platform/ext/target/nxp/common/crypto_accelerator_config.h
        tf-m/platform/ext/target/nxp/common/libc_dummy.c
        tf-m/platform/ext/target/nxp/common/crypto_hw.c
        tf-m/platform/ext/target/nxp/common/mpc_ppc_faults.c
        tf-m/platform/ext/target/nxp/common/services/src/tfm_platform_system.c
        tf-m/platform/ext/target/nxp/common/mmio_defs.h
        tf-m/platform/ext/target/nxp/common/plat_huk.c
        tf-m/platform/ext/target/nxp/common/tfm_hal_isolation.c
        tf-m/platform/ext/target/nxp/common/tfm_hal_platform.c
        tf-m/platform/ext/target/nxp/common/Device/Config/device_cfg.h
        tf-m/platform/ext/target/nxp/common/Device/Include/cmsis.h
        tf-m/platform/ext/target/nxp/common/Device/Include/device_definition.h
        tf-m/platform/ext/target/nxp/common/Device/Include/platform_description.h
        tf-m/platform/ext/target/nxp/common/Device/Include/platform_irq.h
        tf-m/platform/ext/target/nxp/common/Device/Include/platform_regs.h
        tf-m/platform/include/cmsis_override.h
        tf-m/platform/include/exception_info.h
        tf-m/platform/include/mbedtls_entropy_nv_seed_config.h
        tf-m/platform/include/region.h
        tf-m/platform/include/tfm_attest_hal.h
        tf-m/platform/include/tfm_hal_defs.h
        tf-m/platform/include/tfm_hal_device_header.h
        tf-m/platform/include/tfm_hal_interrupt.h
        tf-m/platform/include/tfm_hal_isolation.h
        tf-m/platform/include/tfm_hal_its.h
        tf-m/platform/include/tfm_hal_platform.h
        tf-m/platform/include/tfm_hal_ps.h
        tf-m/platform/include/tfm_plat_boot_seed.h
        tf-m/platform/include/tfm_plat_config.h
        tf-m/platform/include/tfm_plat_crypto_keys.h
        tf-m/platform/include/tfm_plat_defs.h
        tf-m/platform/include/tfm_plat_device_id.h
        tf-m/platform/include/tfm_plat_ns.h
        tf-m/platform/include/tfm_plat_nv_counters.h
        tf-m/platform/include/tfm_plat_otp.h
        tf-m/platform/include/tfm_plat_provisioning.h
        tf-m/platform/include/tfm_plat_test.h
        tf-m/platform/include/tfm_platform_system.h
        tf-m/interface/include/crypto_keys/tfm_builtin_key_ids.h
        tf-m/interface/include/psa/client.h
        tf-m/interface/include/psa/tfm/crypto.h
        tf-m/interface/include/psa/crypto_client_struct.h
        tf-m/interface/include/psa/tfm/crypto_compat.h
        tf-m/interface/include/psa/tfm/crypto_extra.h
        tf-m/interface/include/psa/tfm/crypto_platform.h
        tf-m/interface/include/psa/tfm/crypto_sizes.h
        tf-m/interface/include/psa/tfm/crypto_struct.h
        tf-m/interface/include/psa/tfm/crypto_types.h
        tf-m/interface/include/psa/tfm/crypto_values.h
        tf-m/interface/include/psa/error.h
        tf-m/interface/include/psa/framework_feature.h
        tf-m/interface/include/psa/initial_attestation.h
        tf-m/interface/include/psa/internal_trusted_storage.h
        tf-m/interface/include/psa/lifecycle.h
        tf-m/interface/include/psa/protected_storage.h
        tf-m/interface/include/psa/service.h
        tf-m/interface/include/psa/storage_common.h
        tf-m/interface/include/config_impl.h
        tf-m/interface/include/tfm_api.h
        tf-m/interface/include/tfm_crypto_defs.h
        tf-m/interface/include/tfm_ns_client_ext.h
        tf-m/interface/include/tfm_platform_api.h
        tf-m/interface/include/tfm_psa_call_pack.h
        tf-m/interface/include/psa_manifest/pid.h
        tf-m/secure_fw/spm/include/boot/tfm_boot_status.h
        tf-m/secure_fw/spm/include/interface/runtime_defs.h
        tf-m/secure_fw/spm/include/interface/svc_num.h
        tf-m/secure_fw/spm/include/aapcs_local.h
        tf-m/secure_fw/spm/include/bitops.h
        tf-m/secure_fw/spm/include/critical_section.h
        tf-m/secure_fw/spm/include/config_spm.h
        tf-m/secure_fw/spm/include/current.h
        tf-m/secure_fw/spm/include/lists.h
        tf-m/secure_fw/spm/include/tfm_core_trustzone.h
        tf-m/secure_fw/spm/include/tfm_nspm.h
        tf-m/secure_fw/spm/include/tfm_spm_log.h
        tf-m/secure_fw/spm/include/tfm_version.h
        tf-m/secure_fw/spm/include/utilities.h
        tf-m/secure_fw/spm/include/tfm_arch.h
        tf-m/secure_fw/spm/include/tfm_arch_v8m.h
        tf-m/secure_fw/spm/include/ffm/agent_api.h
        tf-m/secure_fw/spm/include/ffm/backend.h
        tf-m/secure_fw/spm/include/ffm/backend_ipc.h
        tf-m/secure_fw/spm/include/ffm/backend_sfn.h
        tf-m/secure_fw/spm/include/ffm/psa_api.h
        tf-m/secure_fw/spm/include/load/asset_defs.h
        tf-m/secure_fw/spm/include/load/interrupt_defs.h
        tf-m/secure_fw/spm/include/load/partition_defs.h
        tf-m/secure_fw/spm/include/load/service_defs.h
        tf-m/secure_fw/spm/include/load/spm_load_api.h
        tf-m/secure_fw/spm/core/backend_ipc.c
        tf-m/secure_fw/spm/core/psa_api.c
        tf-m/secure_fw/spm/core/psa_call_api.c
        tf-m/secure_fw/spm/core/psa_connection_api.c
        tf-m/secure_fw/spm/core/psa_irq_api.c
        tf-m/secure_fw/spm/core/psa_mmiovec_api.c
        tf-m/secure_fw/spm/core/psa_read_write_skip_api.c
        tf-m/secure_fw/spm/core/psa_version_api.c
        tf-m/secure_fw/spm/core/interrupt.c
        tf-m/secure_fw/spm/core/interrupt.h
        tf-m/secure_fw/spm/core/spm_connection_pool.c
        tf-m/secure_fw/spm/core/spm_log.c
        tf-m/secure_fw/spm/core/stack_watermark.c
        tf-m/secure_fw/spm/core/stack_watermark.h
        tf-m/secure_fw/spm/core/tfm_boot_data.c
        tf-m/secure_fw/spm/core/tfm_boot_data.h
        tf-m/secure_fw/spm/core/utilities.c
        tf-m/secure_fw/spm/ns_client_ext/tfm_ns_ctx.h
        tf-m/secure_fw/spm/ns_client_ext/tfm_spm_ns_ctx.c
        tf-m/secure_fw/include/array.h
        tf-m/secure_fw/include/build_config_check.h
        tf-m/secure_fw/include/compiler_ext_defs.h
        tf-m/secure_fw/include/config_tfm.h
        tf-m/config/config_base.h
        tf-m/secure_fw/include/security_defs.h
        tf-m/secure_fw/partitions/lib/runtime/include/service_api.h
        tf-m/secure_fw/partitions/lib/runtime/include/tfm_sp_log.h
        tf-m/secure_fw/partitions/lib/runtime/include/tfm_strnlen.h
        tf-m/secure_fw/partitions/lib/runtime/service_api.c
        tf-m/secure_fw/partitions/lib/runtime/sfn_common_thread.c
        tf-m/secure_fw/partitions/lib/runtime/sprt_partition_metadata_indicator.c
        tf-m/secure_fw/partitions/lib/runtime/sprt_partition_metadata_indicator.h
        tf-m/secure_fw/partitions/lib/runtime/tfm_sp_log_raw.c
        tf-m/secure_fw/partitions/lib/runtime/crt_strnlen.c
        tf-m/secure_fw/partitions/lib/runtime/psa_api_ipc.c
        tf-m/interface/include/psa_manifest/sid.h
        tf-m/interface/src/tfm_psa_call.c
        tf-m/secure_fw/partitions/ns_agent_tz/load_info_ns_agent_tz.c
        tf-m/secure_fw/partitions/ns_agent_tz/ns_agent_tz_v80m.c
        tf-m/secure_fw/partitions/ns_agent_tz/psa_api_veneers_v80m.c
        tf-m/secure_fw/partitions/idle_partition/idle_partition.c
        tf-m/secure_fw/partitions/idle_partition/load_info_idle_sp.c
        tf-m/secure_fw/spm/core/internal_status_code.h
        tf-m/secure_fw/spm/core/main.c
        tf-m/secure_fw/spm/core/memory_symbols.h
        tf-m/secure_fw/spm/core/tfm_pools.h
        tf-m/secure_fw/spm/core/tfm_rpc.h
        tf-m/secure_fw/spm/core/tfm_svcalls.h
        tf-m/secure_fw/spm/core/arch/tfm_arch.c
        tf-m/secure_fw/spm/core/arch/tfm_arch_v8m_main.c
        tf-m/secure_fw/spm/core/psa_interface_svc.c
        tf-m/secure_fw/spm/core/rom_loader.c
        tf-m/secure_fw/spm/core/spm_ipc.c
        tf-m/secure_fw/spm/core/spm.h
        tf-m/secure_fw/spm/core/tfm_svcalls.c
        tf-m/secure_fw/spm/core/tfm_pools.c
        tf-m/secure_fw/spm/core/thread.c
        tf-m/secure_fw/spm/core/thread.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/
        tf-m/lib/fih/inc
        tf-m/platform/ext 
        tf-m/platform/ext/common 
        tf-m/platform/ext/common/template 
        tf-m/platform/ext/driver 
        tf-m/platform/ext/target/arm/drivers/mpu/armv8m 
        tf-m/platform/ext/target/nxp/common 
        tf-m/platform/ext/accelerator/interface 
        tf-m/platform/ext/target/nxp/common/Device/Config 
        tf-m/platform/ext/target/nxp/common/Device/Include 
        tf-m/platform/include
        tf-m/interface/include/psa
        tf-m/interface/include
        tf-m/interface/include/crypto_keys
        tf-m/interface/include/psa_manifest
        tf-m/secure_fw/include
        tf-m/secure_fw/spm
        tf-m/secure_fw/spm/include
        tf-m/secure_fw/spm/core
        tf-m/secure_fw/spm/include/boot
        tf-m/secure_fw/spm/include/ffm
        tf-m/secure_fw/spm/include/interface
        tf-m/secure_fw/partitions
        tf-m/secure_fw/partitions/protected_storage/psa_manifest
        tf-m/secure_fw/partitions/crypto/psa_manifest
        tf-m/secure_fw/partitions/
        tf-m/secure_fw/partitions/initial_attestation/psa_manifest
        tf-m/secure_fw/partitions/internal_trusted_storage/psa_manifest
        tf-m/secure_fw/partitions/lib/runtime/include
        tf-m/config
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_mdk_configuration(
        LD "--library_type=standardlib"
    )
endif()

# ------- add secure/non-secure board configs start ---------

# include board cmake
include(${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/mcux.cmake OPTIONAL)

# ------- add secure/non-secure board configs END ---------

#adding tfm profile related configs when using secure build
if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_large.s_configs)
    mcux_add_macro(
         CC "-DTFM_LVL=2\
            -DPROJECT_CONFIG_HEADER_FILE=\\\"config_profile_large.h\\\"\
            -DMBEDTLS_CONFIG_FILE=\\\"tfm_mbedcrypto_config_profile_large.h\\\"\
            -DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=\\\"crypto_config_profile_large.h\\\"\
            -DPS_CRYPTO_AEAD_ALG=PSA_ALG_CCM\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_profile_large.h 
        tf-m/lib/ext/mbedcrypto/mbedcrypto_config/crypto_config_profile_large.h 
        tf-m/config/profile/config_profile_large.h 
        tf-m/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_extra_nv_seed.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        tf-m/config/profile
        tf-m/lib/ext/mbedcrypto/mbedcrypto_config 
        tf-m/platform/ext/target/nxp/common
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_medium.s_configs)
    mcux_add_macro(
         CC "-DTFM_LVL=2\
            -DPROJECT_CONFIG_HEADER_FILE=\\\"config_profile_medium.h\\\"\
            -DMBEDTLS_CONFIG_FILE=\\\"tfm_mbedcrypto_config_profile_medium.h\\\"\
            -DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=\\\"crypto_config_profile_medium.h\\\"\
            -DPS_CRYPTO_AEAD_ALG=PSA_ALG_CCM\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_profile_medium.h 
        tf-m/lib/ext/mbedcrypto/mbedcrypto_config/crypto_config_profile_medium.h 
        tf-m/config/profile/config_profile_medium.h 
        tf-m/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_extra_nv_seed.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/lib/ext/mbedcrypto/mbedcrypto_config 
        tf-m/platform/ext/target/nxp/common 
        tf-m/config/profile
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_small.s_configs)
    mcux_add_macro(
         CC "-DTFM_LVL=1\
            -DPROJECT_CONFIG_HEADER_FILE=\\\"config_profile_small.h\\\"\
            -DMBEDTLS_CONFIG_FILE=\\\"tfm_mbedcrypto_config_profile_small.h\\\"\
            -DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=\\\"crypto_config_profile_small.h\\\"\
            -DSYMMETRIC_INITIAL_ATTESTATION\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_profile_small.h 
        tf-m/lib/ext/mbedcrypto/mbedcrypto_config/crypto_config_profile_small.h 
        tf-m/config/profile/config_profile_small.h 
        tf-m/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_extra_nv_seed.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/lib/ext/mbedcrypto/mbedcrypto_config 
        tf-m/platform/ext/target/nxp/common 
        tf-m/config/profile
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.platform_common)
    mcux_add_macro(
    
        CC "-DPLATFORM_DEFAULT_NV_COUNTERS\
           "
    )
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/common/template/crypto_keys.c 
        tf-m/platform/ext/common/template/nv_counters.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES tf-m/platform/include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.loader_service)
    mcux_add_macro(
        CC "-DTFM_PARTITION_LOADER_SERVICE\
        "
    )
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/interface/include/tfm_lds_defs.h 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/interface/src/tfm_lds_api.c 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/auto_generated/intermedia_tfm_loader_service.c 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/auto_generated/load_info_tfm_loader_service.c 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/psa_manifest/tfm_loader_service.h 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/tfm_lds_pid.h 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/tfm_lds_sid.h 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/tfm_lds_config.h 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/tfm_lds_func_ids.h 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/tfm_lds_req_mngr.h 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/tfm_lds_req_mngr.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/psa_manifest 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/interface/include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.platform_rw61x)
    mcux_add_macro(
        CC "-DUSE_ELS_PKC_HUK\
          -DUSE_ELS_PKC_IAK\
          -DHARDENING_MACROS_ENABLED\
          -DPSA_WANT_ECC_SECP_R1_224\
        "
    )
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/rw61x/crypto_keys.c 
        tf-m/platform/ext/target/nxp/rw61x/nv_counters.c 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_USART.c 
        tf-m/platform/ext/target/nxp/rw61x/platform/include/platform_nv_counters_ids.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/platform/include
        tf-m/platform/ext/target/nxp/rw61x/
        tf-m/platform/ext/target/nxp/rw61x/platform/include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.flash_iap1)
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash_iap1.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.flash_iap)
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash_iap.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.flash_iap_n4a)
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash_iap_n4a.c 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash_flexspi_n4a.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.flash_mcxn)
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash_iap_n4a.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.romapi_rw61x)
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash_iap_rw61x.c 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash_iap_rw61x_iped.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.flash_k4)
    mcux_add_macro(
         CC "-DFLASH_DRIVER_IS_FLASH_RESIDENT=0\
           "
    )
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash_k4.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.flash_k4_romapi)
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash_k4.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.lpuart)
    mcux_add_macro(
         CC "-DTFM_LPUART_FEATRUE\
           "
    )
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_LPUART.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.usart)
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_USART.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.platform)
    mcux_add_macro(
         CC "-DTFM_PARTITION_PLATFORM\
           "
    )
    mcux_add_source(
        SOURCES 
        tf-m/interface/src/tfm_platform_api.c 
        tf-m/secure_fw/partitions/platform/auto_generated/intermedia_tfm_platform.c 
        tf-m/secure_fw/partitions/platform/auto_generated/load_info_tfm_platform.c 
        tf-m/secure_fw/partitions/platform/platform_sp.h 
        tf-m/secure_fw/partitions/platform/psa_manifest/tfm_platform.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES tf-m/secure_fw/partitions/platform
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.platform.sp_common)
    mcux_add_source(
        SOURCES 
        tf-m/secure_fw/partitions/platform/platform_sp.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.platform.sp_rw61x)
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/rw61x/platform_sp.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/secure_fw/partitions/platform
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.crypto)
    mcux_add_macro(
         CC "-DTFM_PARTITION_CRYPTO\
            -DPLATFORM_DEFAULT_CRYPTO_KEYS\
            -DMBEDTLS_PSA_CRYPTO_DRIVERS\
            -DMBEDTLS_PSA_CRYPTO_BUILTIN_KEYS\
            -DPSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY\
            -DPSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/interface/src/tfm_crypto_api.c
        tf-m/secure_fw/partitions/crypto/auto_generated/intermedia_tfm_crypto.c
        tf-m/secure_fw/partitions/crypto/auto_generated/load_info_tfm_crypto.c
        tf-m/secure_fw/partitions/crypto/psa_driver_api/tfm_builtin_key_loader.c
        tf-m/secure_fw/partitions/crypto/psa_driver_api/tfm_builtin_key_loader.h
        tf-m/secure_fw/partitions/crypto/config_engine_buf.h
        tf-m/secure_fw/partitions/crypto/config_crypto_check.h
        tf-m/secure_fw/partitions/crypto/crypto_aead.c
        tf-m/secure_fw/partitions/crypto/crypto_alloc.c
        tf-m/secure_fw/partitions/crypto/crypto_asymmetric.c
        tf-m/secure_fw/partitions/crypto/crypto_check_config.h
        tf-m/secure_fw/partitions/crypto/crypto_cipher.c
        tf-m/secure_fw/partitions/crypto/crypto_hash.c
        tf-m/secure_fw/partitions/crypto/crypto_init.c
        tf-m/secure_fw/partitions/crypto/crypto_key_derivation.c
        tf-m/secure_fw/partitions/crypto/crypto_key_management.c
        tf-m/secure_fw/partitions/crypto/crypto_library.c
        tf-m/secure_fw/partitions/crypto/crypto_library.h
        tf-m/secure_fw/partitions/crypto/crypto_mac.c
        tf-m/secure_fw/partitions/crypto/crypto_rng.c
        tf-m/secure_fw/partitions/crypto/crypto_spe.h
        tf-m/secure_fw/partitions/crypto/tfm_crypto_api.h
        tf-m/secure_fw/partitions/crypto/tfm_crypto_key.h
        tf-m/secure_fw/partitions/crypto/tfm_mbedcrypto_include.h
        tf-m/secure_fw/partitions/crypto/psa_manifest/tfm_crypto.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/secure_fw/partitions/crypto 
        tf-m/secure_fw/partitions/crypto/psa_driver_api
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.initial_attestation.asymmetric)
    mcux_add_macro(
         CC "-DATTEST_KEY_BITS=256\
            -DT_COSE_DISABLE_MAC0\
            -DT_COSE_DISABLE_ES384\
            -DT_COSE_DISABLE_ES512\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/secure_fw/partitions/initial_attestation/attest_asymmetric_key.c
        tf-m/platform/ext/common/template/tfm_initial_attest_pub_key.c
        tf-m/lib/ext/t_cose/src/t_cose_sign1_sign.c
        tf-m/lib/ext/t_cose/inc/t_cose_sign1_sign.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/lib/ext/t_cose/src 
        tf-m/lib/ext/t_cose/inc
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.initial_attestation.symmetric)
    mcux_add_macro(
         CC "-DT_COSE_DISABLE_SIGN1\
            -DT_COSE_DISABLE_ES384\
            -DT_COSE_DISABLE_ES512\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/secure_fw/partitions/initial_attestation/attest_symmetric_key.c
        tf-m/lib/ext/t_cose/src/t_cose_mac0_sign.c
        tf-m/lib/ext/t_cose/inc/t_cose_mac0_sign.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/lib/ext/t_cose/src 
        tf-m/lib/ext/t_cose/inc
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.initial_attestation)
    mcux_add_macro(
         CC "-DTFM_PARTITION_INITIAL_ATTESTATION\
            -DT_COSE_USE_PSA_CRYPTO\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/interface/include/tfm_attest_defs.h
        tf-m/interface/include/tfm_attest_iat_defs.h
        tf-m/interface/src/tfm_attest_api.c
        tf-m/secure_fw/partitions/initial_attestation/auto_generated/intermedia_tfm_initial_attestation.c
        tf-m/secure_fw/partitions/initial_attestation/auto_generated/load_info_tfm_initial_attestation.c
        tf-m/secure_fw/partitions/initial_attestation/attest.h
        tf-m/secure_fw/partitions/initial_attestation/attest_boot_data.c
        tf-m/secure_fw/partitions/initial_attestation/attest_boot_data.h
        tf-m/secure_fw/partitions/initial_attestation/attest_core.c
        tf-m/secure_fw/partitions/initial_attestation/attest_key.h
        tf-m/secure_fw/partitions/initial_attestation/attest_token.h
        tf-m/secure_fw/partitions/initial_attestation/attest_token_encode.c
        tf-m/secure_fw/partitions/initial_attestation/tfm_attest.c
        tf-m/secure_fw/partitions/initial_attestation/tfm_attest_req_mngr.c
        tf-m/secure_fw/partitions/initial_attestation/psa_manifest/tfm_initial_attestation.h
        tf-m/lib/ext/t_cose/crypto_adapters/t_cose_psa_crypto.c
        tf-m/lib/ext/t_cose/src/t_cose_crypto.h
        tf-m/lib/ext/t_cose/src/t_cose_parameters.c
        tf-m/lib/ext/t_cose/src/t_cose_parameters.h
        tf-m/lib/ext/t_cose/src/t_cose_standard_constants.h
        tf-m/lib/ext/t_cose/src/t_cose_util.c
        tf-m/lib/ext/t_cose/src/t_cose_util.h
        tf-m/lib/ext/t_cose/inc/t_cose_common.h
        tf-m/lib/ext/t_cose/inc/q_useful_buf.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/secure_fw/partitions/initial_attestation 
        tf-m/lib/ext/t_cose/src 
        tf-m/lib/ext/t_cose/inc
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.qcbor)
    mcux_add_macro(
         CC "-DQCBOR_DISABLE_FLOAT_HW_USE\
            -DUSEFULBUF_DISABLE_ALL_FLOAT\
            -DQCBOR_DISABLE_PREFERRED_FLOAT\
            "
    )
    mcux_add_source(
        SOURCES 
        qcbor/src/ieee754.c 
        qcbor/src/ieee754.h 
        qcbor/src/qcbor_decode.c 
        qcbor/src/qcbor_encode.c 
        qcbor/src/UsefulBuf.c 
        qcbor/inc/qcbor/qcbor.h 
        qcbor/inc/qcbor/qcbor_common.h 
        qcbor/inc/qcbor/qcbor_decode.h 
        qcbor/inc/qcbor/qcbor_encode.h 
        qcbor/inc/qcbor/qcbor_private.h 
        qcbor/inc/qcbor/qcbor_spiffy_decode.h 
        qcbor/inc/qcbor/UsefulBuf.h 
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES qcbor/src qcbor/inc qcbor/inc/qcbor
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.qcbor.test)
    mcux_add_macro(
         CC "-DQCBOR_DISABLE_FLOAT_HW_USE\
            -DUSEFULBUF_DISABLE_ALL_FLOAT\
            -DQCBOR_DISABLE_PREFERRED_FLOAT\
            "
    )
    mcux_add_source(
        SOURCES 
        qcbor/test/float_tests.h 
        qcbor/test/qcbor_encode_tests.c 
        qcbor/test/qcbor_encode_tests.h 
        qcbor/test/not_well_formed_cbor.h 
        qcbor/test/qcbor_decode_tests.c 
        qcbor/test/qcbor_decode_tests.h 
        qcbor/test/run_tests.c 
        qcbor/test/run_tests.h 
        qcbor/test/UsefulBuf_Tests.c 
        qcbor/test/UsefulBuf_Tests.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES qcbor/test
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.ps)
    mcux_add_macro(
         CC "-DTFM_PARTITION_PROTECTED_STORAGE\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/interface/include/tfm_ps_defs.h
        tf-m/interface/src/tfm_ps_api.c
        tf-m/secure_fw/partitions/protected_storage/auto_generated/intermedia_tfm_protected_storage.c
        tf-m/secure_fw/partitions/protected_storage/auto_generated/load_info_tfm_protected_storage.c
        tf-m/secure_fw/partitions/protected_storage/crypto/ps_crypto_interface.c
        tf-m/secure_fw/partitions/protected_storage/crypto/ps_crypto_interface.h
        tf-m/secure_fw/partitions/protected_storage/config_ps_check.h
        tf-m/secure_fw/partitions/protected_storage/ps_encrypted_object.c
        tf-m/secure_fw/partitions/protected_storage/ps_encrypted_object.h
        tf-m/secure_fw/partitions/protected_storage/ps_object_defs.h
        tf-m/secure_fw/partitions/protected_storage/ps_object_system.c
        tf-m/secure_fw/partitions/protected_storage/ps_object_system.h
        tf-m/secure_fw/partitions/protected_storage/ps_object_table.c
        tf-m/secure_fw/partitions/protected_storage/ps_object_table.h
        tf-m/secure_fw/partitions/protected_storage/ps_utils.c
        tf-m/secure_fw/partitions/protected_storage/ps_utils.h
        tf-m/secure_fw/partitions/protected_storage/tfm_protected_storage.c
        tf-m/secure_fw/partitions/protected_storage/tfm_protected_storage.h
        tf-m/secure_fw/partitions/protected_storage/tfm_ps_req_mngr.h
        tf-m/secure_fw/partitions/protected_storage/tfm_ps_req_mngr.c
        tf-m/secure_fw/partitions/protected_storage/psa_manifest/tfm_protected_storage.h
        tf-m/secure_fw/partitions/protected_storage/nv_counters/ps_nv_counters.h
        tf-m/secure_fw/partitions/protected_storage/nv_counters/ps_nv_counters.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/interface/include
        tf-m/secure_fw/partitions/protected_storage/crypto
        tf-m/secure_fw/partitions/protected_storage
        tf-m/secure_fw/partitions/protected_storage/nv_counters
        tf-m/secure_fw/partitions/platform
        tf-m/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.its)
    mcux_add_macro(
         CC "-DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/interface/include/tfm_its_defs.h
        tf-m/interface/src/tfm_its_api.c
        tf-m/secure_fw/partitions/internal_trusted_storage/auto_generated/intermedia_tfm_internal_trusted_storage.c
        tf-m/secure_fw/partitions/internal_trusted_storage/its_utils.c
        tf-m/secure_fw/partitions/internal_trusted_storage/its_utils.h
        tf-m/secure_fw/partitions/internal_trusted_storage/tfm_internal_trusted_storage.c
        tf-m/secure_fw/partitions/internal_trusted_storage/tfm_internal_trusted_storage.h
        tf-m/secure_fw/partitions/internal_trusted_storage/tfm_its_req_mngr.c
        tf-m/secure_fw/partitions/internal_trusted_storage/tfm_its_req_mngr.h
        tf-m/secure_fw/partitions/internal_trusted_storage/psa_manifest/tfm_internal_trusted_storage.h
        tf-m/secure_fw/partitions/internal_trusted_storage/flash/its_flash.c
        tf-m/secure_fw/partitions/internal_trusted_storage/flash/its_flash.h
        tf-m/secure_fw/partitions/internal_trusted_storage/flash/its_flash_nand.c
        tf-m/secure_fw/partitions/internal_trusted_storage/flash/its_flash_nand.h
        tf-m/secure_fw/partitions/internal_trusted_storage/flash/its_flash_nor.c
        tf-m/secure_fw/partitions/internal_trusted_storage/flash/its_flash_nor.h
        tf-m/secure_fw/partitions/internal_trusted_storage/flash/its_flash_ram.c
        tf-m/secure_fw/partitions/internal_trusted_storage/flash/its_flash_ram.h
        tf-m/secure_fw/partitions/internal_trusted_storage/flash_fs/its_flash_fs.c
        tf-m/secure_fw/partitions/internal_trusted_storage/flash_fs/its_flash_fs.h
        tf-m/secure_fw/partitions/internal_trusted_storage/flash_fs/its_flash_fs_dblock.c
        tf-m/secure_fw/partitions/internal_trusted_storage/flash_fs/its_flash_fs_dblock.h
        tf-m/secure_fw/partitions/internal_trusted_storage/flash_fs/its_flash_fs_mblock.c
        tf-m/secure_fw/partitions/internal_trusted_storage/flash_fs/its_flash_fs_mblock.h
        tf-m/secure_fw/partitions/protected_storage/ps_object_defs.h
        tf-m/secure_fw/partitions/protected_storage/config_ps_check.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/interface/include 
        tf-m/secure_fw/partitions/internal_trusted_storage 
        tf-m/secure_fw/partitions/internal_trusted_storage/flash 
        tf-m/secure_fw/partitions/internal_trusted_storage/flash_fs 
        tf-m/secure_fw/partitions/protected_storage 
        tf-m/secure_fw/partitions/platform
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.its.load_info_common)
    mcux_add_source(
        SOURCES 
        tf-m/secure_fw/partitions/internal_trusted_storage/auto_generated/load_info_tfm_internal_trusted_storage.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.its.load_info_rw61x)
    mcux_add_source(
        SOURCES tf-m/platform/ext/target/nxp/rw61x/load_info_tfm_internal_trusted_storage.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.os_wrapper.baremetal)
    mcux_add_source(
        SOURCES 
        tf-m/interface/src/os_wrapper/tfm_ns_interface_bare_metal.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.os_wrapper.rtos)
    mcux_add_source(
        SOURCES 
        tf-m/interface/src/os_wrapper/tfm_ns_interface_rtos.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.mbedtls_compatibility)
    mcux_add_source(
        SOURCES 
        tf-m/interface/include/psa/tfm/psa/crypto.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/interface/include/psa/tfm
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns)
    mcux_add_macro(
         CC "-DCONFIG_TFM_FLOAT_ABI=2\
            -DCONFIG_TFM_ENABLE_CP10CP11\
            -DCONFIG_TFM_LAZY_STACKING\
            -D__DOMAIN_NS=1\
            -DDOMAIN_NS=1\
            -DCONFIG_TFM_USE_TRUSTZONE\
            -DATTEST_TOKEN_PROFILE_PSA_IOT_1\
            -DPLATFORM_DEFAULT_CRYPTO_KEYS\
            -DPS_ENCRYPTION\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/platform/include/tfm_plat_crypto_keys.h 
        tf-m/platform/ext/common/uart_stdout.h 
        tf-m/platform/ext/common/uart_stdout.c 
        tf-m/platform/ext/driver/Driver_USART.h 
        tf-m/platform/ext/driver/Driver_Common.h 
        tf-m/platform/ext/target/nxp/common/libc_dummy.c 
        tf-m/platform/ext/target/nxp/common/Device/Include/cmsis.h 
        tf-m/platform/ext/target/nxp/common/Device/Include/device_definition.h 
        tf-m/platform/ext/target/nxp/common/Device/Include/platform_irq.h 
        tf-m/platform/ext/target/nxp/common/Device/Config/device_cfg.h 
        tf-m/platform/ext/target/nxp/common/Device/Include/platform_regs.h 
        tf-m/platform/include/tfm_hal_device_header.h 
        tf-m/platform/include/tfm_plat_boot_seed.h 
        tf-m/platform/include/tfm_plat_device_id.h 
        tf-m/platform/include/tfm_plat_defs.h 
        tf-m/platform/include/tfm_plat_ns.h 
        tf-m/platform/include/region.h 
        tf-m/platform/include/tfm_plat_test.h 
        tf-m/interface/include/crypto_keys/tfm_builtin_key_ids.h 
        tf-m/interface/include/psa_manifest/sid.h 
        tf-m/interface/include/tfm_attest_defs.h 
        tf-m/interface/include/config_impl.h 
        tf-m/interface/include/tfm_crypto_defs.h 
        tf-m/interface/include/tfm_its_defs.h 
        tf-m/interface/include/tfm_ps_defs.h 
        tf-m/interface/include/tfm_api.h 
        tf-m/interface/include/tfm_ns_interface.h 
        tf-m/interface/include/tfm_psa_call_pack.h 
        tf-m/interface/include/tfm_ns_client_ext.h 
        tf-m/interface/include/tfm_platform_api.h 
        tf-m/interface/include/os_wrapper/common.h 
        tf-m/interface/include/os_wrapper/mutex.h 
        tf-m/interface/include/psa/client.h 
        tf-m/interface/include/psa/tfm/crypto.h 
        tf-m/interface/include/psa/tfm/crypto_compat.h 
        tf-m/interface/include/psa/crypto_client_struct.h 
        tf-m/interface/include/psa/tfm/crypto_extra.h 
        tf-m/interface/include/psa/tfm/crypto_platform.h 
        tf-m/interface/include/psa/tfm/crypto_sizes.h 
        tf-m/interface/include/psa/tfm/crypto_struct.h 
        tf-m/interface/include/psa/tfm/crypto_types.h 
        tf-m/interface/include/psa/tfm/crypto_values.h 
        tf-m/interface/include/psa/error.h 
        tf-m/interface/include/psa/framework_feature.h 
        tf-m/interface/include/psa/initial_attestation.h 
        tf-m/interface/include/psa/internal_trusted_storage.h 
        tf-m/interface/include/psa/lifecycle.h 
        tf-m/interface/include/psa/protected_storage.h 
        tf-m/interface/include/psa/service.h 
        tf-m/interface/include/psa/storage_common.h 
        tf-m/secure_fw/spm/include/tfm_arch.h 
        tf-m/secure_fw/spm/include/tfm_arch_v8m.h 
        tf-m/secure_fw/spm/include/utilities.h 
        tf-m/secure_fw/include/config_tfm.h 
        tf-m/config/config_base.h 
        tf-m/secure_fw/partitions/crypto/config_crypto_check.h 
        tf-m/secure_fw/partitions/protected_storage/config_ps_check.h 
        tf-m/interface/src/tfm_crypto_api.c 
        tf-m/interface/src/tfm_attest_api.c 
        tf-m/interface/src/tfm_its_api.c 
        tf-m/interface/src/tfm_ps_api.c 
        tf-m/interface/src/tfm_platform_api.c 
        tf-m/interface/src/tfm_psa_ns_api.c 
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/platform/include 
        tf-m/platform/ext 
        tf-m/platform/ext/common 
        tf-m/platform/ext/driver 
        tf-m/platform/ext/target/nxp/common/Device/Config 
        tf-m/platform/ext/target/nxp/common/Device/Include 
        tf-m/interface/include 
        tf-m/interface/include/crypto_keys 
        tf-m/interface/include/psa 
        tf-m/interface/include/psa_manifest 
        tf-m/interface/include/os_wrapper 
        tf-m/secure_fw/spm/include 
        tf-m/secure_fw/include 
        tf-m/secure_fw/partitions/crypto 
        tf-m/secure_fw/partitions/initial_attestation 
        tf-m/secure_fw/partitions/internal_trusted_storage 
        tf-m/secure_fw/partitions/protected_storage 
        tf-m/secure_fw/partitions/platform 
        tf-m/config
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

#adding tfm profile related configs when using non-secure build
if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_large.ns_configs)
    mcux_add_macro(
         CC "-DTFM_LVL=2\
            -DPROJECT_CONFIG_HEADER_FILE=\\\"config_profile_large.h\\\"\
            "
    )
    mcux_add_source(
        SOURCES tf-m/config/profile/config_profile_large.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES tf-m/config/profile
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_medium.ns_configs)
    mcux_add_macro(
         CC "-DTFM_LVL=2\
            -DPROJECT_CONFIG_HEADER_FILE=\\\"config_profile_medium.h\\\"\
            "
    )
    mcux_add_source(
        SOURCES tf-m/config/profile/config_profile_medium.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES tf-m/config/profile
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_small.ns_configs)
    mcux_add_macro(
         CC "-DTFM_LVL=1\
            -DPROJECT_CONFIG_HEADER_FILE=\\\"config_profile_small.h\\\"\
            "
    )
    mcux_add_source(
        SOURCES tf-m/config/profile/config_profile_small.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES tf-m/config/profile
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.loader_service)
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/interface/include/tfm_lds_defs.h 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/interface/src/tfm_lds_api.c 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/tfm_lds_sid.h 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/tfm_lds_func_ids.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service 
        tf-m/platform/ext/target/nxp/rw61x/partitions/loader_service/interface/include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.platform_rw61x)
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_USART.c 
        tf-m/platform/ext/target/nxp/rw61x/plat_iak_pk/plat_iak_pk.c 
        tf-m/platform/ext/target/nxp/rw61x/plat_iak_pk/plat_iak_pk.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES tf-m/platform/ext/target/nxp/rw61x/plat_iak_pk
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfmtests.ns)
    mcux_add_source(
        SOURCES 
        tf-m-tests/app/os_wrapper_cmsis_rtos_v2.c 
        tf-m-tests/app/main_ns.c 
        tf-m-tests/app/test_app.c 
        tf-m-tests/app/test_app.h 
        tf-m-tests/app/tfm_log.h 
        tf-m-tests/log/tfm_log_raw.c 
        tf-m-tests/log/tfm_log_raw.h 
        tf-m-tests/ns_interface/os_wrapper/delay.h 
        tf-m-tests/ns_interface/os_wrapper/msg_queue.h 
        tf-m-tests/ns_interface/os_wrapper/semaphore.h 
        tf-m-tests/ns_interface/os_wrapper/thread.h 
        tf-m-tests/ns_interface/ns_client_ext/tfm_nsid_manager.h 
        tf-m-tests/ns_interface/ns_client_ext/tz_shim_layer.c 
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m-tests/app 
        tf-m-tests/log 
        tf-m-tests/ns_interface 
        tf-m-tests/ns_interface/ns_client_ext
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.test)
    mcux_add_macro(
         CC "-DTFM_S_REG_TEST\
            -DUSE_SP_LOG\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m-tests/test/secure_fw/secure_suites.c 
        tf-m-tests/test/framework/test_framework.c 
        tf-m-tests/test/framework/test_framework.h 
        tf-m-tests/test/framework/test_framework_error_codes.h 
        tf-m-tests/test/framework/test_framework_helpers.c 
        tf-m-tests/test/framework/test_framework_helpers.h 
        tf-m-tests/test/framework/test_framework_integ_test.h 
        tf-m-tests/test/framework/test_framework_integ_test_helper.c 
        tf-m-tests/test/framework/test_framework_integ_test_helper.h 
        tf-m-tests/test/framework/test_log.h 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_secure_client_service/auto_generated/intermedia_tfm_secure_client_service.c 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_secure_client_service/auto_generated/load_info_tfm_secure_client_service.c 
        tf-m-tests/test/secure_fw/common_test_services/tfm_secure_client_service/tfm_secure_client_service.c 
        tf-m-tests/test/secure_fw/common_test_services/tfm_secure_client_service/tfm_secure_client_service.h 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_secure_client_service/psa_manifest/tfm_secure_client_service.h 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_secure_client_2/auto_generated/intermedia_tfm_secure_client_2.c 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_secure_client_2/auto_generated/load_info_tfm_secure_client_2.c 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_secure_client_2/psa_manifest/tfm_secure_client_2.h 
        tf-m-tests/test/secure_fw/common_test_services/tfm_secure_client_2/tfm_secure_client_2.c 
        tf-m-tests/test/secure_fw/common_test_services/tfm_secure_client_2/tfm_secure_client_2_api.c 
        tf-m-tests/test/secure_fw/common_test_services/tfm_secure_client_2/tfm_secure_client_2_api.h 
        tf-m-tests/test/secure_fw/suites/spm/ipc/secure/ipc_s_tests.h 
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES ./ 
        tf-m-tests/test 
        tf-m-tests/test/framework 
        tf-m-tests/test/secure_fw/common_test_services/tfm_secure_client_service 
        tf-m-tests/test/secure_fw/common_test_services/tfm_secure_client_2 
        tf-m-tests/test/secure_fw/suites/spm/ipc/secure 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_secure_client_service/psa_manifest 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_secure_client_service 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_secure_client_2/psa_manifest 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_secure_client_2
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

#adding configs based upon selected profile
if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_large.s_test_configs)
    mcux_add_macro(
         CC "-DTFM_CRYPTO_TEST_ALG_CBC\
            -DTFM_CRYPTO_TEST_ALG_CCM\
            -DTFM_CRYPTO_TEST_ALG_GCM\
            -DTFM_CRYPTO_TEST_ALG_SHA_512\
            -DTFM_CRYPTO_TEST_HKDF\
            -DTFM_CRYPTO_TEST_ECDH\
            -DTFM_CRYPTO_TEST_UNSUPPORTED_ALG\
            "
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_medium.s_test_configs)
    mcux_add_macro(
         CC "-DTFM_CRYPTO_TEST_ALG_CCM\
            -DTFM_CRYPTO_TEST_ECDH\
            -DTFM_CRYPTO_TEST_UNSUPPORTED_ALG\
            "
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_small.s_test_configs)
    mcux_add_macro(
         CC "-DTFM_CRYPTO_TEST_ALG_CCM\
            -DTFM_CRYPTO_TEST_UNSUPPORTED_ALG\
            "
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.test.ps)
    mcux_add_macro(
         CC "-DTFM_PARTITION_TEST_PS\
            -DTEST_S_PS\
            "
    )
    mcux_add_source(
        SOURCES tf-m-tests/test/secure_fw/suites/ps/secure/psa_ps_s_interface_testsuite.c
       tf-m-tests/test/secure_fw/suites/ps/secure/psa_ps_s_reliability_testsuite.c
       tf-m-tests/test/secure_fw/suites/ps/secure/s_test_helpers.h
       tf-m-tests/test/secure_fw/suites/ps/secure/ps_tests.h
       tf-m/autogenerated/test/secure_fw/test_services/tfm_ps_test_service/auto_generated/intermedia_tfm_ps_test_service.c
       tf-m/autogenerated/test/secure_fw/test_services/tfm_ps_test_service/auto_generated/load_info_tfm_ps_test_service.c
       tf-m/autogenerated/test/secure_fw/test_services/tfm_ps_test_service/psa_manifest/tfm_ps_test_service.h
       tf-m-tests/test/secure_fw/suites/ps/service/tfm_ps_test_service.c
       tf-m-tests/test/secure_fw/suites/ps/service/tfm_ps_test_service_api.c
       tf-m-tests/test/secure_fw/suites/ps/service/tfm_ps_test_service_api.h
       BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m-tests/test/secure_fw/suites/ps/secure 
        tf-m-tests/test/secure_fw/suites/ps/service 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_ps_test_service/psa_manifest
        tf-m/autogenerated/test/secure_fw/test_services/tfm_ps_test_service
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.test.its)
    mcux_add_macro(
         CC "-DTEST_S_ITS\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m-tests/test/secure_fw/suites/its/its_tests_common.c 
        tf-m-tests/test/secure_fw/suites/its/its_tests_common.h 
        tf-m-tests/test/secure_fw/suites/its/secure/its_s_tests.h 
        tf-m-tests/test/secure_fw/suites/its/secure/psa_its_s_interface_testsuite.c 
        tf-m-tests/test/secure_fw/suites/its/secure/psa_its_s_reliability_testsuite.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m-tests/test/secure_fw/suites/its 
        tf-m-tests/test/secure_fw/suites/its/secure
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.test.attestation.asymmetric)
    mcux_add_source(
        SOURCES 
        tf-m-tests/test/secure_fw/suites/attestation/secure/attest_asymmetric_s_interface_testsuite.c 
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_decode_asymmetric.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.test.attestation.symmetric)
    mcux_add_source(
        SOURCES 
        tf-m-tests/test/secure_fw/suites/attestation/secure/attest_symmetric_s_interface_testsuite.c 
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_decode_symmetric.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.test.attestation)
    mcux_add_macro(
         CC "-DTEST_S_ATTESTATION\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m-tests/test/secure_fw/suites/attestation/ext/qcbor_util/qcbor_util.c 
        tf-m-tests/test/secure_fw/suites/attestation/ext/qcbor_util/qcbor_util.h 
        tf-m-tests/test/secure_fw/suites/attestation/secure/attest_s_tests.h 
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_decode.h 
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_decode_common.c 
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_test.c 
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_test.h 
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_test_values.h 
        tf-m-tests/test/secure_fw/suites/attestation/attest_tests_common.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m-tests/test/secure_fw/suites/attestation/ext/qcbor_util 
        tf-m-tests/test/secure_fw/suites/attestation/secure 
        tf-m-tests/test/secure_fw/suites/attestation
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.t_cose.sign1_verify)
    mcux_add_source(
        SOURCES 
        tf-m/lib/ext/t_cose/src/t_cose_sign1_verify.c 
        tf-m/lib/ext/t_cose/inc/t_cose_sign1_verify.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/lib/ext/t_cose/inc
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.t_cose.mac0_verify)
    mcux_add_source(
        SOURCES 
        tf-m/lib/ext/t_cose/src/t_cose_mac0_verify.c 
        tf-m/lib/ext/t_cose/inc/t_cose_mac0_verify.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m/lib/ext/t_cose/inc
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.test.crypto)
    mcux_add_macro(
         CC "-DTEST_S_CRYPTO\
            "
    )
    mcux_add_source(
        SOURCES tf-m-tests/test/secure_fw/suites/crypto/crypto_tests_common.c 
        tf-m-tests/test/secure_fw/suites/crypto/crypto_tests_common.h 
        tf-m-tests/test/secure_fw/suites/crypto/crypto_tests_check_config.h 
        tf-m-tests/test/secure_fw/suites/crypto/secure/crypto_s_tests.h 
        tf-m-tests/test/secure_fw/suites/crypto/secure/crypto_sec_interface_testsuite.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m-tests/test/secure_fw/suites/crypto 
        tf-m-tests/test/secure_fw/suites/crypto/secure
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.platform_test)
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/target/nxp/common/plat_test.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.test.platform)
    mcux_add_macro(
         CC "-DTEST_S_PLATFORM\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m-tests/test/secure_fw/suites/platform/platform_tests_common.c 
        tf-m-tests/test/secure_fw/suites/platform/platform_tests_common.h 
        tf-m-tests/test/secure_fw/suites/platform/secure/platform_s_tests.h 
        tf-m-tests/test/secure_fw/suites/platform/secure/platform_s_interface_testsuite.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES tf-m-tests/test/secure_fw/suites/platform 
        tf-m-tests/test/secure_fw/suites/platform/secure
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.test.ipc)
    mcux_add_macro(
         CC "-DTFM_PARTITION_IPC_TEST\
            -DTEST_S_IPC\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m-tests/test/secure_fw/suites/spm/common/spm_test_defs.h
       tf-m-tests/test/secure_fw/suites/spm/common/suites/client_api_tests.c
       tf-m-tests/test/secure_fw/suites/spm/common/suites/client_api_tests.h
       tf-m-tests/test/secure_fw/suites/spm/common/service/client_api_test_defs.h
       tf-m-tests/test/secure_fw/suites/spm/common/service/client_api_test_service.c
       tf-m-tests/test/secure_fw/suites/spm/common/service/client_api_test_service.h
       tf-m-tests/test/secure_fw/suites/spm/ipc/secure/ipc_s_tests.h
       tf-m-tests/test/secure_fw/suites/spm/ipc/secure/ipc_s_interface_testsuite.c
       tf-m/autogenerated/test/secure_fw/test_services/tfm_ipc_service/auto_generated/intermedia_tfm_ipc_service_test.c
       tf-m/autogenerated/test/secure_fw/test_services/tfm_ipc_service/auto_generated/load_info_tfm_ipc_service_test.c
       tf-m/autogenerated/test/secure_fw/test_services/tfm_ipc_service/psa_manifest/tfm_ipc_service_test.h
       tf-m-tests/test/secure_fw/suites/spm/ipc/service/tfm_ipc_service/tfm_ipc_service_test.c
       tf-m/autogenerated/test/secure_fw/test_services/tfm_ipc_client/auto_generated/intermedia_tfm_ipc_client_test.c
       tf-m/autogenerated/test/secure_fw/test_services/tfm_ipc_client/auto_generated/load_info_tfm_ipc_client_test.c
       tf-m/autogenerated/test/secure_fw/test_services/tfm_ipc_client/psa_manifest/tfm_ipc_client_test.h
       tf-m-tests/test/secure_fw/suites/spm/ipc/service/tfm_ipc_client/tfm_ipc_client_test.c
       BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        tf-m-tests/test/secure_fw/suites/spm/ipc/secure
        tf-m/autogenerated/test/secure_fw/test_services/tfm_ipc_service/psa_manifest
        tf-m/autogenerated/test/secure_fw/test_services/tfm_ipc_service
        tf-m-tests/test/secure_fw/suites/spm/common
        tf-m-tests/test/secure_fw/suites/spm/common/suites
        tf-m-tests/test/secure_fw/suites/spm/common/service
        tf-m-tests/test/secure_fw/suites/spm/ipc/service/tfm_ipc_service
        tf-m/autogenerated/test/secure_fw/test_services/tfm_ipc_client/psa_manifest
        tf-m/autogenerated/test/secure_fw/test_services/tfm_ipc_client
        tf-m-tests/test/secure_fw/suites/spm/ipc/service/tfm_ipc_client
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.test.irq_slih)
    mcux_add_macro(
         CC "-DTEST_NS_SLIH_IRQ\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m-tests/test/secure_fw/suites/spm/common/spm_test_defs.h 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_slih_test_service/auto_generated/intermedia_tfm_slih_test_service.c
        tf-m/autogenerated/test/secure_fw/test_services/tfm_slih_test_service/auto_generated/load_info_tfm_slih_test_service.c 
        tf-m-tests/test/secure_fw/suites/spm/irq/service/tfm_slih_test_service/tfm_slih_test_service.c 
        tf-m-tests/test/secure_fw/suites/spm/common/service/tfm_irq_test_service.c 
        tf-m-tests/test/secure_fw/suites/spm/common/service/tfm_irq_test_service.h 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_slih_test_service/psa_manifest/tfm_slih_test_service.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m-tests/test/secure_fw/suites/spm/irq/service/tfm_slih_test_service 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_slih_test_service 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_slih_test_service/psa_manifest 
        tf-m-tests/test/secure_fw/suites/spm/common/ 
        tf-m-tests/test/secure_fw/suites/spm/common/service
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.s.test.irq_flih)
    mcux_add_macro(
         CC "-DTEST_NS_FLIH_IRQ\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m-tests/test/secure_fw/suites/spm/common/spm_test_defs.h 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_flih_test_service/auto_generated/intermedia_tfm_flih_test_service.c
        tf-m/autogenerated/test/secure_fw/test_services/tfm_flih_test_service/auto_generated/load_info_tfm_flih_test_service.c 
        tf-m-tests/test/secure_fw/suites/spm/irq/service/tfm_flih_test_service/tfm_flih_test_service.c 
        tf-m-tests/test/secure_fw/suites/spm/common/service/tfm_irq_test_service.c 
        tf-m-tests/test/secure_fw/suites/spm/common/service/tfm_irq_test_service.h 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_flih_test_service/psa_manifest/tfm_flih_test_service.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m-tests/test/secure_fw/suites/spm/irq/service/tfm_flih_test_service 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_flih_test_service 
        tf-m/autogenerated/test/secure_fw/test_services/tfm_flih_test_service/psa_manifest 
        tf-m-tests/test/secure_fw/suites/spm/common/ 
        tf-m-tests/test/secure_fw/suites/spm/common/service
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test)
    mcux_add_macro(
         CC "-DTFM_NS_REG_TEST\
            "
    )
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/non_secure_suites.c
        tf-m-tests/test/framework/test_framework.c
        tf-m-tests/test/framework/test_framework.h
        tf-m-tests/test/framework/test_framework_error_codes.h
        tf-m-tests/test/framework/test_framework_integ_test.h
        tf-m-tests/test/framework/test_framework_integ_test_helper.c
        tf-m-tests/test/framework/test_framework_integ_test_helper.h
        tf-m-tests/test/framework/test_framework_helpers.c
        tf-m-tests/test/framework/test_framework_helpers.h
        tf-m-tests/test/framework/test_log.h
        tf-m-tests/test/secure_fw/suites/spm/ipc/non_secure/ipc_ns_tests.h
        tf-m-tests/test/secure_fw/suites/t_cose/non_secure/t_cose_ns_tests.h
        tf-m-tests/test/secure_fw/suites/multi_core/non_secure/multi_core_ns_test.h
        tf-m-tests/test/secure_fw/suites/ps/non_secure/ps_ns_tests.h
        tf-m-tests/test/secure_fw/suites/qcbor/non_secure/qcbor_ns_tests.h
        tf-m-tests/test/secure_fw/suites/platform/non_secure/platform_ns_tests.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        tf-m-tests/test/framework
        tf-m-tests/test/secure_fw/suites/spm/ipc/non_secure
        tf-m-tests/test/secure_fw/suites/t_cose/non_secure
        tf-m-tests/test/secure_fw/suites/multi_core/non_secure
        tf-m-tests/test/secure_fw/suites/ps/non_secure
        tf-m-tests/test/secure_fw/suites/qcbor/non_secure
        tf-m-tests/test/secure_fw/suites/platform/non_secure 
        tf-m-tests/
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_large.ns_test_configs)
    mcux_add_macro(
         CC "-DTFM_CRYPTO_TEST_ALG_CBC\
            -DTFM_CRYPTO_TEST_ALG_CCM\
            -DTFM_CRYPTO_TEST_ALG_GCM\
            -DTFM_CRYPTO_TEST_ALG_SHA_512\
            -DTFM_CRYPTO_TEST_HKDF\
            -DTFM_CRYPTO_TEST_ECDH\
            -DTFM_CRYPTO_TEST_UNSUPPORTED_ALG\
            "
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_medium.ns_test_configs)
    mcux_add_macro(
         CC "-DTFM_CRYPTO_TEST_ALG_CCM\
            -DTFM_CRYPTO_TEST_ECDH\
            -DTFM_CRYPTO_TEST_UNSUPPORTED_ALG\
            "
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_small.ns_test_configs)
    mcux_add_macro(
         CC "-DTFM_CRYPTO_TEST_ALG_CCM\
            -DTFM_CRYPTO_TEST_UNSUPPORTED_ALG\
            "
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.ps)
    mcux_add_macro(
         CC "-DTEST_NS_PS\
            "
    )
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/suites/ps/non_secure/ns_test_helpers.c
        tf-m-tests/test/secure_fw/suites/ps/non_secure/ns_test_helpers.h
        tf-m-tests/test/secure_fw/suites/ps/non_secure/psa_ps_ns_interface_testsuite.c
        tf-m-tests/test/secure_fw/suites/ps/non_secure/ps_ns_tests.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m-tests/test/secure_fw/suites/ps/non_secure
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.its)
    mcux_add_macro(
         CC "-DTEST_NS_ITS\
            "
    )
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/suites/its/its_tests_common.c
        tf-m-tests/test/secure_fw/suites/its/its_tests_common.h
        tf-m-tests/test/secure_fw/suites/its/non_secure/psa_its_ns_interface_testsuite.c
        tf-m-tests/test/secure_fw/suites/its/non_secure/its_ns_tests.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m-tests/test/secure_fw/suites/its 
        tf-m-tests/test/secure_fw/suites/its/non_secure
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.s)
    mcux_add_macro(
         CC "-DTFM_S_REG_TEST\
            "
    )
    mcux_add_include(
        INCLUDES tf-m-tests/test/secure_fw/common_test_services/tfm_secure_client_service
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.attestation.asymmetric)
    mcux_add_source(
        SOURCES 
        tf-m/platform/ext/common/template/tfm_initial_attest_pub_key.c 
        tf-m/lib/ext/t_cose/src/t_cose_sign1_sign.c 
        tf-m/lib/ext/t_cose/inc/t_cose_sign1_sign.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES tf-m/lib/ext/t_cose/inc
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.attestation.symmetric)
    mcux_add_macro(
         CC "-DSYMMETRIC_INITIAL_ATTESTATION\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/lib/ext/t_cose/src/t_cose_mac0_sign.c
        tf-m/lib/ext/t_cose/inc/t_cose_mac0_sign.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES tf-m/lib/ext/t_cose/inc
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.attestation)
    mcux_add_macro(
         CC "-DT_COSE_USE_PSA_CRYPTO\
            "
    )
    mcux_add_source(
        SOURCES 
        tf-m/interface/include/tfm_attest_iat_defs.h 
        tf-m/secure_fw/partitions/initial_attestation/attest.h 
        tf-m/secure_fw/partitions/initial_attestation/attest_token.h 
        tf-m/secure_fw/partitions/initial_attestation/attest_key.h 
        tf-m/secure_fw/spm/include/boot/tfm_boot_status.h 
        tf-m/lib/ext/t_cose/src/t_cose_crypto.h 
        tf-m/lib/ext/t_cose/crypto_adapters/t_cose_psa_crypto.c 
        tf-m/lib/ext/t_cose/src/t_cose_parameters.c 
        tf-m/lib/ext/t_cose/src/t_cose_parameters.h 
        tf-m/lib/ext/t_cose/src/t_cose_sign1_sign.c 
        tf-m/lib/ext/t_cose/src/t_cose_standard_constants.h 
        tf-m/lib/ext/t_cose/src/t_cose_util.c 
        tf-m/lib/ext/t_cose/src/t_cose_util.h 
        tf-m/lib/ext/t_cose/inc/t_cose_common.h 
        tf-m/lib/ext/t_cose/inc/q_useful_buf.h 
        tf-m/lib/ext/t_cose/inc/t_cose_sign1_sign.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES tf-m/interface/include 
        tf-m/secure_fw/partitions/initial_attestation 
        tf-m/secure_fw/spm/include 
        tf-m/secure_fw/spm/include/boot 
        tf-m/lib/ext/t_cose/src 
        tf-m/lib/ext/t_cose/inc
        tf-m/.
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfmtests.ns.test.attestation.asymmetric)
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/suites/attestation/non_secure/attest_asymmetric_ns_interface_testsuite.c
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_decode_asymmetric.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfmtests.ns.test.attestation.symmetric)
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/suites/attestation/non_secure/attest_symmetric_ns_interface_testsuite.c
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_decode_symmetric.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfmtests.ns.test.attestation)
    mcux_add_macro(
         CC "-DTEST_NS_ATTESTATION\
            "
    )
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/suites/attestation/ext/qcbor_util/qcbor_util.c
        tf-m-tests/test/secure_fw/suites/attestation/ext/qcbor_util/qcbor_util.h
        tf-m-tests/test/secure_fw/suites/attestation/non_secure/attest_ns_tests.h
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_decode.h
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_decode_common.c
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_test.c
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_test.h
        tf-m-tests/test/secure_fw/suites/attestation/attest_token_test_values.h
        tf-m-tests/test/secure_fw/suites/attestation/attest_tests_common.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m-tests/test/secure_fw/suites/attestation/ext/qcbor_util 
        tf-m-tests/test/secure_fw/suites/attestation/non_secure 
        tf-m-tests/test/secure_fw/suites/attestation
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.crypto)
    mcux_add_macro(
         CC "-DTEST_NS_CRYPTO\
            -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE\
            "
    )
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/suites/crypto/crypto_tests_common.c
        tf-m-tests/test/secure_fw/suites/crypto/crypto_tests_common.h
        tf-m-tests/test/secure_fw/suites/crypto/crypto_tests_check_config.h
        tf-m-tests/test/secure_fw/suites/crypto/non_secure/crypto_ns_interface_testsuite.c
        tf-m-tests/test/secure_fw/suites/crypto/non_secure/crypto_ns_tests.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        tf-m-tests/test/secure_fw/suites/crypto 
        tf-m-tests/test/secure_fw/suites/crypto/non_secure
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.qcbor)
    mcux_add_macro(
         CC "-DTEST_NS_QCBOR\
            "
    )
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/suites/qcbor/non_secure/qcbor_ns_testsuite.c
        tf-m-tests/test/secure_fw/suites/qcbor/non_secure/qcbor_ns_tests.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES tf-m-tests/test/secure_fw/suites/qcbor/non_secure
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.ipc)
    mcux_add_macro(
         CC "-DTFM_PARTITION_IPC_TEST\
            -DTEST_NS_IPC\
            "
    )
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/suites/spm/common/service/client_api_test_defs.h
        tf-m-tests/test/secure_fw/suites/spm/common/suites/client_api_tests.c
        tf-m-tests/test/secure_fw/suites/spm/common/suites/client_api_tests.h
        tf-m-tests/test/secure_fw/suites/spm/ipc/non_secure/ipc_ns_interface_testsuite.c
        tf-m-tests/test/secure_fw/suites/spm/ipc/non_secure/ipc_ns_tests.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        tf-m-tests/test/secure_fw/suites/spm/common/suites
        tf-m-tests/test/secure_fw/suites/spm/common/service
        tf-m-tests/test/secure_fw/suites/spm/ipc/non_secure
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.irq_slih)
    mcux_add_macro(
         CC "-DTEST_NS_SLIH_IRQ\
            "
    )
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/suites/spm/common/spm_test_defs.h
        tf-m-tests/test/secure_fw/suites/spm/common/service/client_api_test_defs.h
        tf-m-tests/test/secure_fw/suites/spm/common/suites/irq_test.c
        tf-m-tests/test/secure_fw/suites/spm/common/suites/irq_test.h
        tf-m-tests/test/secure_fw/suites/spm/irq/irq_testsuite.c
        tf-m-tests/test/secure_fw/suites/spm/irq/irq_testsuite.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        tf-m-tests/test/secure_fw/suites/spm/common/suites
        tf-m-tests/test/secure_fw/suites/spm/common/service
        tf-m-tests/test/secure_fw/suites/spm/irq
        tf-m-tests/test/secure_fw/suites/spm/common
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.irq_flih)
    mcux_add_macro(
         CC "-DTEST_NS_FLIH_IRQ\
            "
    )
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/suites/spm/common/spm_test_defs.h
        tf-m-tests/test/secure_fw/suites/spm/common/service/client_api_test_defs.h
        tf-m-tests/test/secure_fw/suites/spm/common/suites/irq_test.c
        tf-m-tests/test/secure_fw/suites/spm/common/suites/irq_test.h
        tf-m-tests/test/secure_fw/suites/spm/irq/irq_testsuite.c
        tf-m-tests/test/secure_fw/suites/spm/irq/irq_testsuite.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        tf-m-tests/test/secure_fw/suites/spm/common/suites
        tf-m-tests/test/secure_fw/suites/spm/common/service
        tf-m-tests/test/secure_fw/suites/spm/irq
        tf-m-tests/test/secure_fw/suites/spm/common
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.platform)
    mcux_add_macro(
         CC "-DTEST_NS_PLATFORM\
            "
    )
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/suites/platform/platform_tests_common.c
        tf-m-tests/test/secure_fw/suites/platform/platform_tests_common.h
        tf-m-tests/test/secure_fw/suites/platform/non_secure/platform_ns_tests.h
        tf-m-tests/test/secure_fw/suites/platform/non_secure/platform_ns_interface_testsuite.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        tf-m-tests/test/secure_fw/suites/platform
        tf-m-tests/test/secure_fw/suites/platform/non_secure
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.ns.test.tcose)
    mcux_add_macro(
         CC "-DTEST_NS_T_COSE\
            -DT_COSE_DISABLE_ES384\
            -DT_COSE_DISABLE_ES512\
            -DT_COSE_DISABLE_SIGN_VERIFY_TESTS\
            -DT_COSE_USE_PSA_CRYPTO_FROM_TFM\
            "
    )
    mcux_add_source(
        SOURCES
        tf-m/lib/ext/t_cose/test/t_cose_make_test_messages.c
        tf-m/lib/ext/t_cose/test/t_cose_make_test_messages.h
        tf-m/lib/ext/t_cose/test/t_cose_make_psa_test_key.c
        tf-m/lib/ext/t_cose/test/t_cose_make_test_pub_key.h
        tf-m/lib/ext/t_cose/test/t_cose_sign_verify_test.c
        tf-m/lib/ext/t_cose/test/t_cose_sign_verify_test.h
        tf-m/lib/ext/t_cose/test/t_cose_test.c
        tf-m/lib/ext/t_cose/test/t_cose_test.h
        tf-m/lib/ext/t_cose/test/run_tests.c
        tf-m/lib/ext/t_cose/test/run_tests.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        tf-m/lib/ext/t_cose/test
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfmtests.ns.test.tcose)
    mcux_add_source(
        SOURCES
        tf-m-tests/test/secure_fw/suites/t_cose/non_secure/t_cose_ns_tests.h
        tf-m-tests/test/secure_fw/suites/t_cose/non_secure/t_cose_ns_testsuite.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        tf-m-tests/test/secure_fw/suites/t_cose/non_secure
        tf-m-tests/test/secure_fw/suites/t_cose
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.mbedcrypto)
    mcux_add_source(
        SOURCES 
        mbed-crypto/include/mbedtls/aes.h
        mbed-crypto/include/mbedtls/aria.h
        mbed-crypto/include/mbedtls/asn1.h
        mbed-crypto/include/mbedtls/asn1write.h
        mbed-crypto/include/mbedtls/base64.h
        mbed-crypto/include/mbedtls/bignum.h
        mbed-crypto/include/mbedtls/build_info.h
        mbed-crypto/include/mbedtls/camellia.h
        mbed-crypto/include/mbedtls/ccm.h
        mbed-crypto/include/mbedtls/chacha20.h
        mbed-crypto/include/mbedtls/chachapoly.h
        mbed-crypto/include/mbedtls/check_config.h
        mbed-crypto/include/mbedtls/cipher.h
        mbed-crypto/include/mbedtls/compat-2.x.h
        mbed-crypto/include/mbedtls/cmac.h
        mbed-crypto/include/mbedtls/config_psa.h
        mbed-crypto/include/mbedtls/constant_time.h
        mbed-crypto/include/mbedtls/ctr_drbg.h
        mbed-crypto/include/mbedtls/debug.h
        mbed-crypto/include/mbedtls/des.h
        mbed-crypto/include/mbedtls/dhm.h
        mbed-crypto/include/mbedtls/ecdh.h
        mbed-crypto/include/mbedtls/ecdsa.h
        mbed-crypto/include/mbedtls/ecjpake.h
        mbed-crypto/include/mbedtls/ecp.h
        mbed-crypto/include/mbedtls/entropy.h
        mbed-crypto/include/mbedtls/error.h
        mbed-crypto/include/mbedtls/gcm.h
        mbed-crypto/include/mbedtls/hkdf.h
        mbed-crypto/include/mbedtls/hmac_drbg.h
        mbed-crypto/include/mbedtls/legacy_or_psa.h
        mbed-crypto/include/mbedtls/lms.h
        mbed-crypto/include/mbedtls/md.h
        mbed-crypto/include/mbedtls/md5.h
        mbed-crypto/include/mbedtls/memory_buffer_alloc.h
        mbed-crypto/include/mbedtls/nist_kw.h
        mbed-crypto/include/mbedtls/oid.h
        mbed-crypto/include/mbedtls/pem.h
        mbed-crypto/include/mbedtls/pk.h
        mbed-crypto/include/mbedtls/pkcs5.h
        mbed-crypto/include/mbedtls/pkcs12.h
        mbed-crypto/include/mbedtls/platform.h
        mbed-crypto/include/mbedtls/platform_time.h
        mbed-crypto/include/mbedtls/platform_util.h
        mbed-crypto/include/mbedtls/poly1305.h
        mbed-crypto/include/mbedtls/private_access.h
        mbed-crypto/include/mbedtls/psa_util.h
        mbed-crypto/include/mbedtls/ripemd160.h
        mbed-crypto/include/mbedtls/rsa.h
        mbed-crypto/include/mbedtls/sha1.h
        mbed-crypto/include/mbedtls/sha256.h
        mbed-crypto/include/mbedtls/sha512.h
        mbed-crypto/include/mbedtls/threading.h
        mbed-crypto/include/mbedtls/timing.h
        mbed-crypto/include/mbedtls/version.h
        mbed-crypto/include/psa/crypto.h
        mbed-crypto/include/psa/crypto_builtin_composites.h
        mbed-crypto/include/psa/crypto_builtin_primitives.h
        mbed-crypto/include/psa/crypto_compat.h
        mbed-crypto/include/psa/crypto_config.h
        mbed-crypto/include/psa/crypto_driver_common.h
        mbed-crypto/include/psa/crypto_extra.h
        mbed-crypto/include/psa/crypto_platform.h
        mbed-crypto/include/psa/crypto_se_driver.h
        mbed-crypto/include/psa/crypto_sizes.h
        mbed-crypto/include/psa/crypto_struct.h
        mbed-crypto/include/psa/crypto_types.h
        mbed-crypto/include/psa/crypto_values.h
        mbed-crypto/include/psa/crypto_driver_contexts_composites.h
        mbed-crypto/include/psa/crypto_driver_contexts_primitives.h
        mbed-crypto/library/aes.c
        mbed-crypto/library/aesce.c
        mbed-crypto/library/aesce.h
        mbed-crypto/library/aesni.c
        mbed-crypto/library/aesni.h
        mbed-crypto/library/alignment.h
        mbed-crypto/library/asn1parse.c
        mbed-crypto/library/asn1write.c
        mbed-crypto/library/base64.c
        mbed-crypto/library/bignum.c
        mbed-crypto/library/bignum_core.c
        mbed-crypto/library/bignum_core.h
        mbed-crypto/library/bignum_mod.c
        mbed-crypto/library/bignum_mod.h
        mbed-crypto/library/bignum_mod_raw.c
        mbed-crypto/library/bignum_mod_raw.h
        mbed-crypto/library/bignum_mod_raw_invasive.h
        mbed-crypto/library/bn_mul.h
        mbed-crypto/library/camellia.c
        mbed-crypto/library/ccm.c
        mbed-crypto/library/chacha20.c
        mbed-crypto/library/chachapoly.c
        mbed-crypto/library/check_crypto_config.h
        mbed-crypto/library/cipher.c
        mbed-crypto/library/cipher_wrap.c
        mbed-crypto/library/cipher_wrap.h
        mbed-crypto/library/cmac.c
        mbed-crypto/library/common.h
        mbed-crypto/library/constant_time.c
        mbed-crypto/library/constant_time_internal.h
        mbed-crypto/library/constant_time_invasive.h
        mbed-crypto/library/ctr_drbg.c
        mbed-crypto/library/debug.c
        mbed-crypto/library/des.c
        mbed-crypto/library/dhm.c
        mbed-crypto/library/ecdh.c
        mbed-crypto/library/ecdsa.c
        mbed-crypto/library/ecjpake.c
        mbed-crypto/library/ecp.c
        mbed-crypto/library/ecp_curves.c
        mbed-crypto/library/ecp_internal_alt.h
        mbed-crypto/library/ecp_invasive.h
        mbed-crypto/library/entropy.c
        mbed-crypto/library/entropy_poll.c
        mbed-crypto/library/entropy_poll.h
        mbed-crypto/library/error.c
        mbed-crypto/library/gcm.c
        mbed-crypto/library/hash_info.c
        mbed-crypto/library/hash_info.h
        mbed-crypto/library/hkdf.c
        mbed-crypto/library/hmac_drbg.c
        mbed-crypto/library/lmots.c
        mbed-crypto/library/lmots.h
        mbed-crypto/library/lms.c
        mbed-crypto/library/md.c
        mbed-crypto/library/md_wrap.h
        mbed-crypto/library/md5.c
        mbed-crypto/library/memory_buffer_alloc.c
        mbed-crypto/library/nist_kw.c
        mbed-crypto/library/oid.c
        mbed-crypto/library/padlock.c
        mbed-crypto/library/padlock.h
        mbed-crypto/library/pem.c
        mbed-crypto/library/pk.c
        mbed-crypto/library/pk_wrap.c
        mbed-crypto/library/pk_wrap.h
        mbed-crypto/library/pkcs5.c
        mbed-crypto/library/pkcs12.c
        mbed-crypto/library/pkparse.c
        mbed-crypto/library/pkwrite.c
        mbed-crypto/library/pkwrite.h
        mbed-crypto/library/platform.c
        mbed-crypto/library/platform_util.c
        mbed-crypto/library/poly1305.c
        mbed-crypto/library/psa_crypto.c
        mbed-crypto/library/psa_crypto_aead.c
        mbed-crypto/library/psa_crypto_aead.h
        mbed-crypto/library/psa_crypto_cipher.c
        mbed-crypto/library/psa_crypto_cipher.h
        mbed-crypto/library/psa_crypto_client.c
        mbed-crypto/library/psa_crypto_core.h
        mbed-crypto/library/psa_crypto_driver_wrappers.h
        mbed-crypto/library/psa_crypto_driver_wrappers.c
        mbed-crypto/library/psa_crypto_ecp.c
        mbed-crypto/library/psa_crypto_ecp.h
        mbed-crypto/library/psa_crypto_hash.c
        mbed-crypto/library/psa_crypto_hash.h
        mbed-crypto/library/psa_crypto_invasive.h
        mbed-crypto/library/psa_crypto_its.h
        mbed-crypto/library/psa_crypto_mac.c
        mbed-crypto/library/psa_crypto_mac.h
        mbed-crypto/library/psa_crypto_pake.c
        mbed-crypto/library/psa_crypto_random_impl.h
        mbed-crypto/library/psa_crypto_rsa.c
        mbed-crypto/library/psa_crypto_rsa.h
        mbed-crypto/library/psa_crypto_pake.h
        mbed-crypto/library/psa_crypto_se.c
        mbed-crypto/library/psa_crypto_se.h
        mbed-crypto/library/psa_crypto_slot_management.c
        mbed-crypto/library/psa_crypto_slot_management.h
        mbed-crypto/library/psa_crypto_storage.c
        mbed-crypto/library/psa_crypto_storage.h
        mbed-crypto/library/psa_its_file.c
        mbed-crypto/library/psa_util.c
        mbed-crypto/library/ripemd160.c
        mbed-crypto/library/rsa.c
        mbed-crypto/library/rsa_alt_helpers.c
        mbed-crypto/library/rsa_alt_helpers.h
        mbed-crypto/library/sha1.c
        mbed-crypto/library/sha256.c
        mbed-crypto/library/sha512.c
        mbed-crypto/library/threading.c
        mbed-crypto/library/timing.c
        mbed-crypto/library/version.c
        mbed-crypto/library/version_features.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        mbed-crypto/include/mbedtls 
        mbed-crypto/include/psa 
        mbed-crypto/library 
        mbed-crypto/port 
        mbed-crypto/include
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.mbedcrypto.port)
    mcux_add_source(
        SOURCES 
        mbed-crypto/port/aes_alt.h 
        mbed-crypto/port/mbedcrypto_mcux_casper.c 
        mbed-crypto/port/mbedcrypto_mcux_els.c 
        mbed-crypto/port/mbedcrypto_mcux_hashcrypt.c 
        mbed-crypto/port/mbedcrypto_mcux_config.h 
        mbed-crypto/port/mbedcrypto_mcux_pkc.c 
        mbed-crypto/port/mbedcrypto_mcux_sss.c 
        mbed-crypto/port/mbedtls_accelerator_config.h 
        mbed-crypto/port/sha1_alt.h port/sha256_alt.h 
        mbed-crypto/port/sha512_alt.h port/ecdsa_alt.h 
        mbed-crypto/port/ecdh_alt.h port/ccm_alt.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES mbed-crypto/port
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.mbedcrypto.port.config)
    mcux_add_macro(
         CC "-DCRYPTO_HW_ACCELERATOR\
            "
    )
    mcux_add_source(
        SOURCES 
        mbed-crypto/port/mbedcrypto_mcux_config.h 
        mbed-crypto/port/mbedtls_accelerator_config.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES mbed-crypto/port
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.mbedcrypto.port.rng)
    mcux_add_source(
        SOURCES 
        mbed-crypto/port/mbedcrypto_mcux_entropy.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES mbed-crypto/port
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

#-------------------- psa-arch-test-------------
if (CONFIG_MCUX_COMPONENT_middleware.psatest.ns)
    mcux_add_source(
        SOURCES 
        psa-arch-tests/api-tests/val/test_entry_list.inc
        psa-arch-tests/api-tests/val/test_entry_fn_declare_list.inc
        psa-arch-tests/api-tests/val/nspe/pal_interfaces_ns.h
        psa-arch-tests/api-tests/val/nspe/val_attestation.c
        psa-arch-tests/api-tests/val/nspe/val_attestation.h
        psa-arch-tests/api-tests/val/nspe/val_crypto.c
        psa-arch-tests/api-tests/val/nspe/val_crypto.h
        psa-arch-tests/api-tests/val/nspe/val_entry.c
        psa-arch-tests/api-tests/val/nspe/val_entry.h
        psa-arch-tests/api-tests/val/nspe/val_framework.c
        psa-arch-tests/api-tests/val/nspe/val_framework.h
        psa-arch-tests/api-tests/val/nspe/val_dispatcher.c
        psa-arch-tests/api-tests/val/nspe/val_dispatcher.h
        psa-arch-tests/api-tests/val/nspe/val_interfaces.c
        psa-arch-tests/api-tests/val/nspe/val_interfaces.h
        psa-arch-tests/api-tests/val/nspe/val_peripherals.c
        psa-arch-tests/api-tests/val/nspe/val_peripherals.h
        psa-arch-tests/api-tests/val/nspe/val_platform.c
        psa-arch-tests/api-tests/val/nspe/val_platform.h
        psa-arch-tests/api-tests/val/nspe/val_storage.c
        psa-arch-tests/api-tests/val/nspe/val_storage.h
        psa-arch-tests/api-tests/val/common/val.h
        psa-arch-tests/api-tests/val/common/val_client_defs.h
        psa-arch-tests/api-tests/val/common/val_target.c
        psa-arch-tests/api-tests/val/common/val_target.h
        psa-arch-tests/api-tests/platform/drivers/nvmem/pal_nvmem.c
        psa-arch-tests/api-tests/platform/drivers/nvmem/pal_nvmem.h
        psa-arch-tests/api-tests/platform/targets/common/nspe/crypto/pal_crypto_config_check.h
        psa-arch-tests/api-tests/platform/targets/common/nspe/pal_common.h
        psa-arch-tests/api-tests/platform/targets/common/nspe/pal_weak.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        psa-arch-tests/api-tests/val/nspe
        psa-arch-tests/api-tests/val/common
        psa-arch-tests/api-tests/platform/drivers/nvmem
        psa-arch-tests/api-tests/platform/targets/common/nspe/crypto
        psa-arch-tests/api-tests/platform/targets/common/nspe
        psa-arch-tests/api-tests/val
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()


if (CONFIG_MCUX_COMPONENT_middleware.psatest.ns.tgt_dev_apis_nxp)
    mcux_add_source(
        SOURCES 
        psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_nxp/nspe/pal_attestation_config.h
        psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_nxp/nspe/pal_config.h
        psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_nxp/nspe/pal_crypto_config.h
        psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_nxp/nspe/pal_driver_intf.c
        psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_nxp/nspe/pal_storage_config.h
        psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_nxp/nspe/target_database.h
        psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_nxp/nspe/target_database.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        psa-arch-tests/api-tests/platform/targets/tgt_dev_apis_nxp/nspe
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.psatest.ns.tgt_dev_apis_nxp_rw61x_configs)
    mcux_add_macro(
         CC "-DDISABLE_ECC_R1_CURVE_TESTS_LESS_THAN_255_BITS\
            "
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.psatest.ns.crypto)
    mcux_add_macro(
         CC "-DCRYPTO\
            "
    )
    mcux_add_source(
        SOURCES 
        psa-arch-tests/api-tests/platform/targets/common/nspe/crypto/pal_crypto_intf.c
        psa-arch-tests/api-tests/platform/targets/common/nspe/crypto/pal_crypto_intf.h
        psa-arch-tests/api-tests/dev_apis/crypto/common/test_crypto_common.c
        psa-arch-tests/api-tests/dev_apis/crypto/common/test_crypto_common.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c001/test_c001.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c001/test_c001.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c001/test_entry_c001.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c002/test_c002.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c002/test_c002.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c002/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c002/test_entry_c002.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c003/test_c003.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c003/test_c003.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c003/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c003/test_entry_c003.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c004/test_c004.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c004/test_c004.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c004/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c004/test_entry_c004.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c005/test_c005.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c005/test_c005.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c005/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c005/test_entry_c005.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c006/test_c006.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c006/test_c006.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c006/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c006/test_entry_c006.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c007/test_c007.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c007/test_c007.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c007/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c007/test_entry_c007.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c008/test_c008.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c008/test_c008.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c008/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c008/test_entry_c008.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c009/test_c009.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c009/test_c009.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c009/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c009/test_entry_c009.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c010/test_c010.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c010/test_c010.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c010/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c010/test_entry_c010.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c011/test_c011.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c011/test_c011.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c011/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c011/test_entry_c011.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c012/test_c012.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c012/test_c012.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c012/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c012/test_entry_c012.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c013/test_c013.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c013/test_c013.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c013/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c013/test_entry_c013.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c014/test_c014.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c014/test_c014.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c014/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c014/test_entry_c014.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c015/test_c015.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c015/test_c015.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c015/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c015/test_entry_c015.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c016/test_c016.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c016/test_c016.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c016/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c016/test_entry_c016.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c017/test_c017.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c017/test_c017.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c017/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c017/test_entry_c017.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c018/test_c018.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c018/test_c018.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c018/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c018/test_entry_c018.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c019/test_c019.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c019/test_c019.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c019/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c019/test_entry_c019.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c020/test_c020.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c020/test_c020.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c020/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c020/test_entry_c020.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c021/test_c021.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c021/test_c021.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c021/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c021/test_entry_c021.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c022/test_c022.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c022/test_c022.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c022/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c022/test_entry_c022.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c023/test_c023.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c023/test_c023.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c023/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c023/test_entry_c023.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c024/test_c024.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c024/test_c024.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c024/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c024/test_entry_c024.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c025/test_c025.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c025/test_c025.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c025/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c025/test_entry_c025.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c026/test_c026.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c026/test_c026.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c026/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c026/test_entry_c026.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c027/test_c027.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c027/test_c027.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c027/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c027/test_entry_c027.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c028/test_c028.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c028/test_c028.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c028/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c028/test_entry_c028.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c029/test_c029.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c029/test_c029.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c029/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c029/test_entry_c029.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c030/test_c030.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c030/test_c030.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c030/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c030/test_entry_c030.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c031/test_c031.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c031/test_c031.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c031/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c031/test_entry_c031.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c032/test_c032.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c032/test_c032.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c032/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c032/test_entry_c032.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c033/test_c033.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c033/test_c033.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c033/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c033/test_entry_c033.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c034/test_c034.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c034/test_c034.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c034/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c034/test_entry_c034.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c035/test_c035.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c035/test_c035.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c035/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c035/test_entry_c035.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c036/test_c036.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c036/test_c036.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c036/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c036/test_entry_c036.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c037/test_c037.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c037/test_c037.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c037/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c037/test_entry_c037.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c038/test_c038.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c038/test_c038.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c038/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c038/test_entry_c038.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c039/test_c039.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c039/test_c039.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c039/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c039/test_entry_c039.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c040/test_c040.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c040/test_c040.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c040/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c040/test_entry_c040.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c041/test_c041.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c041/test_c041.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c041/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c041/test_entry_c041.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c042/test_c042.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c042/test_c042.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c042/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c042/test_entry_c042.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c043/test_c043.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c043/test_c043.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c043/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c043/test_entry_c043.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c044/test_c044.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c044/test_c044.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c044/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c044/test_entry_c044.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c045/test_c045.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c045/test_c045.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c045/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c045/test_entry_c045.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c046/test_c046.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c046/test_c046.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c046/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c046/test_entry_c046.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c047/test_c047.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c047/test_c047.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c047/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c047/test_entry_c047.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c048/test_c048.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c048/test_c048.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c048/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c048/test_entry_c048.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c049/test_c049.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c049/test_c049.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c049/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c049/test_entry_c049.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c050/test_c050.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c050/test_c050.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c050/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c050/test_entry_c050.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c051/test_c051.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c051/test_c051.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c051/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c051/test_entry_c051.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c052/test_c052.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c052/test_c052.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c052/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c052/test_entry_c052.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c053/test_c053.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c053/test_c053.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c053/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c053/test_entry_c053.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c054/test_c054.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c054/test_c054.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c054/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c054/test_entry_c054.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c055/test_c055.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c055/test_c055.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c055/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c055/test_entry_c055.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c056/test_c056.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c056/test_c056.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c056/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c056/test_entry_c056.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c057/test_c057.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c057/test_c057.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c057/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c057/test_entry_c057.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c058/test_c058.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c058/test_c058.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c058/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c058/test_entry_c058.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c059/test_c059.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c059/test_c059.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c059/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c059/test_entry_c059.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c060/test_c060.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c060/test_c060.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c060/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c060/test_entry_c060.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c061/test_c061.c
        psa-arch-tests/api-tests/dev_apis/crypto/test_c061/test_c061.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c061/test_data.h
        psa-arch-tests/api-tests/dev_apis/crypto/test_c061/test_entry_c061.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES 
        psa-arch-tests/api-tests/platform/targets/common/nspe/crypto
        psa-arch-tests/api-tests/dev_apis/crypto/common
        psa-arch-tests/api-tests/dev_apis/crypto/test_c001
        psa-arch-tests/api-tests/dev_apis/crypto/test_c002
        psa-arch-tests/api-tests/dev_apis/crypto/test_c003
        psa-arch-tests/api-tests/dev_apis/crypto/test_c004
        psa-arch-tests/api-tests/dev_apis/crypto/test_c005
        psa-arch-tests/api-tests/dev_apis/crypto/test_c006
        psa-arch-tests/api-tests/dev_apis/crypto/test_c007
        psa-arch-tests/api-tests/dev_apis/crypto/test_c008
        psa-arch-tests/api-tests/dev_apis/crypto/test_c009
        psa-arch-tests/api-tests/dev_apis/crypto/test_c010
        psa-arch-tests/api-tests/dev_apis/crypto/test_c011
        psa-arch-tests/api-tests/dev_apis/crypto/test_c012
        psa-arch-tests/api-tests/dev_apis/crypto/test_c013
        psa-arch-tests/api-tests/dev_apis/crypto/test_c014
        psa-arch-tests/api-tests/dev_apis/crypto/test_c015
        psa-arch-tests/api-tests/dev_apis/crypto/test_c016
        psa-arch-tests/api-tests/dev_apis/crypto/test_c017
        psa-arch-tests/api-tests/dev_apis/crypto/test_c018
        psa-arch-tests/api-tests/dev_apis/crypto/test_c019
        psa-arch-tests/api-tests/dev_apis/crypto/test_c020
        psa-arch-tests/api-tests/dev_apis/crypto/test_c021
        psa-arch-tests/api-tests/dev_apis/crypto/test_c022
        psa-arch-tests/api-tests/dev_apis/crypto/test_c023
        psa-arch-tests/api-tests/dev_apis/crypto/test_c024
        psa-arch-tests/api-tests/dev_apis/crypto/test_c025
        psa-arch-tests/api-tests/dev_apis/crypto/test_c026
        psa-arch-tests/api-tests/dev_apis/crypto/test_c027
        psa-arch-tests/api-tests/dev_apis/crypto/test_c028
        psa-arch-tests/api-tests/dev_apis/crypto/test_c029
        psa-arch-tests/api-tests/dev_apis/crypto/test_c030
        psa-arch-tests/api-tests/dev_apis/crypto/test_c031
        psa-arch-tests/api-tests/dev_apis/crypto/test_c032
        psa-arch-tests/api-tests/dev_apis/crypto/test_c033
        psa-arch-tests/api-tests/dev_apis/crypto/test_c034
        psa-arch-tests/api-tests/dev_apis/crypto/test_c035
        psa-arch-tests/api-tests/dev_apis/crypto/test_c036
        psa-arch-tests/api-tests/dev_apis/crypto/test_c037
        psa-arch-tests/api-tests/dev_apis/crypto/test_c038
        psa-arch-tests/api-tests/dev_apis/crypto/test_c039
        psa-arch-tests/api-tests/dev_apis/crypto/test_c040
        psa-arch-tests/api-tests/dev_apis/crypto/test_c041
        psa-arch-tests/api-tests/dev_apis/crypto/test_c042
        psa-arch-tests/api-tests/dev_apis/crypto/test_c043
        psa-arch-tests/api-tests/dev_apis/crypto/test_c044
        psa-arch-tests/api-tests/dev_apis/crypto/test_c045
        psa-arch-tests/api-tests/dev_apis/crypto/test_c046
        psa-arch-tests/api-tests/dev_apis/crypto/test_c047
        psa-arch-tests/api-tests/dev_apis/crypto/test_c048
        psa-arch-tests/api-tests/dev_apis/crypto/test_c049
        psa-arch-tests/api-tests/dev_apis/crypto/test_c050
        psa-arch-tests/api-tests/dev_apis/crypto/test_c051
        psa-arch-tests/api-tests/dev_apis/crypto/test_c052
        psa-arch-tests/api-tests/dev_apis/crypto/test_c053
        psa-arch-tests/api-tests/dev_apis/crypto/test_c054
        psa-arch-tests/api-tests/dev_apis/crypto/test_c055
        psa-arch-tests/api-tests/dev_apis/crypto/test_c056
        psa-arch-tests/api-tests/dev_apis/crypto/test_c057
        psa-arch-tests/api-tests/dev_apis/crypto/test_c058
        psa-arch-tests/api-tests/dev_apis/crypto/test_c059
        psa-arch-tests/api-tests/dev_apis/crypto/test_c060
        psa-arch-tests/api-tests/dev_apis/crypto/test_c061
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_large.ns_psatest_configs)
    mcux_add_macro(
         CC "-DTF_M_PROFILE_LARGE\
            "
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_medium.ns_psatest_configs)
    mcux_add_macro(
         CC "-DTF_M_PROFILE_MEDIUM\
            "
    )
endif()
if (CONFIG_MCUX_COMPONENT_middleware.tfm.profile_small.ns_psatest_configs)
    mcux_add_macro(
         CC "-DTF_M_PROFILE_SMALL\
            "
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.psatest.ns.initial_attestation)
    mcux_add_macro(
         CC "-DINITIAL_ATTESTATION\
            "
    )
    mcux_add_source(
        SOURCES 
        psa-arch-tests/api-tests/platform/targets/common/nspe/initial_attestation/pal_attestation_crypto.c
        psa-arch-tests/api-tests/platform/targets/common/nspe/initial_attestation/pal_attestation_crypto.h
        psa-arch-tests/api-tests/platform/targets/common/nspe/initial_attestation/pal_attestation_eat.h
        psa-arch-tests/api-tests/platform/targets/common/nspe/initial_attestation/pal_attestation_intf.c
        psa-arch-tests/api-tests/platform/targets/common/nspe/initial_attestation/pal_attestation_intf.h
        psa-arch-tests/api-tests/dev_apis/initial_attestation/test_a001/test_a001.c
        psa-arch-tests/api-tests/dev_apis/initial_attestation/test_a001/test_a001.h
        psa-arch-tests/api-tests/dev_apis/initial_attestation/test_a001/test_data.h
        psa-arch-tests/api-tests/dev_apis/initial_attestation/test_a001/test_entry_a001.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        psa-arch-tests/api-tests/platform/targets/common/nspe/initial_attestation
        psa-arch-tests/api-tests/dev_apis/initial_attestation/test_a001
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()


if (CONFIG_MCUX_COMPONENT_middleware.psatest.ns.t_cose)
    mcux_add_source(
        SOURCES 
        tf-m/lib/ext/t_cose/inc/t_cose_sign1_sign.h
        tf-m/lib/ext/t_cose/inc/t_cose_common.h
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        tf-m/lib/ext/t_cose/inc
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.psatest.ns.protected_storage)
    mcux_add_macro(
         CC "-DPROTECTED_STORAGE\
             -DPS_TEST\
            "
    )
    mcux_add_source(
        SOURCES 
        psa-arch-tests/api-tests/platform/targets/common/nspe/protected_storage/pal_protected_storage_intf.c
        psa-arch-tests/api-tests/platform/targets/common/nspe/protected_storage/pal_protected_storage_intf.h
        psa-arch-tests/api-tests/dev_apis/storage/test_storage_common.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s001/test_s001.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s001/test_s001.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s001/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s001/test_entry_s001.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s002/test_s002.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s002/test_s002.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s002/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s002/test_entry_s002.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s003/test_s003.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s003/test_s003.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s003/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s003/test_entry_s003.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s004/test_s004.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s004/test_s004.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s004/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s004/test_entry_s004.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s005/test_s005.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s005/test_s005.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s005/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s005/test_entry_s005.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s006/test_s006.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s006/test_s006.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s006/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s006/test_entry_s006.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s007/test_s007.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s007/test_s007.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s007/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s007/test_entry_s007.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s008/test_s008.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s008/test_s008.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s008/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s008/test_entry_s008.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s009/test_s009.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s009/test_s009.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s009/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s009/test_entry_s009.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s010/test_s010.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s010/test_s010.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s010/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s010/test_entry_s010.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s011/test_s011.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s011/test_s011.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s011/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s011/test_entry_s011.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s012/test_s012.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s012/test_s012.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s012/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s012/test_entry_s012.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s013/test_s013.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s013/test_s013.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s013/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s013/test_entry_s013.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s014/test_s014.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s014/test_s014.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s014/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s014/test_entry_s014.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s015/test_s015.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s015/test_s015.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s015/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s015/test_entry_s015.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s016/test_s016.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s016/test_s016.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s016/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s016/test_entry_s016.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s017/test_s017.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s017/test_s017.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s017/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s017/test_entry_s017.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/

    )
    mcux_add_include(
        INCLUDES
        psa-arch-tests/api-tests/platform/targets/common/nspe/protected_storage
        psa-arch-tests/api-tests/dev_apis/storage
        psa-arch-tests/api-tests/dev_apis/storage/test_s001
        psa-arch-tests/api-tests/dev_apis/storage/test_s002
        psa-arch-tests/api-tests/dev_apis/storage/test_s003
        psa-arch-tests/api-tests/dev_apis/storage/test_s004
        psa-arch-tests/api-tests/dev_apis/storage/test_s005
        psa-arch-tests/api-tests/dev_apis/storage/test_s006
        psa-arch-tests/api-tests/dev_apis/storage/test_s007
        psa-arch-tests/api-tests/dev_apis/storage/test_s008
        psa-arch-tests/api-tests/dev_apis/storage/test_s009
        psa-arch-tests/api-tests/dev_apis/storage/test_s010
        psa-arch-tests/api-tests/dev_apis/storage/test_s011
        psa-arch-tests/api-tests/dev_apis/storage/test_s012
        psa-arch-tests/api-tests/dev_apis/storage/test_s013
        psa-arch-tests/api-tests/dev_apis/storage/test_s014
        psa-arch-tests/api-tests/dev_apis/storage/test_s015
        psa-arch-tests/api-tests/dev_apis/storage/test_s016
        psa-arch-tests/api-tests/dev_apis/storage/test_s017
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.psatest.ns.internal_trusted_storage)
    mcux_add_macro(
         CC "-DINTERNAL_TRUSTED_STORAGE\
             -DITS_TEST\
            "
    )
    mcux_add_source(
        SOURCES 
        psa-arch-tests/api-tests/platform/targets/common/nspe/internal_trusted_storage/pal_internal_trusted_storage_intf.c
        psa-arch-tests/api-tests/platform/targets/common/nspe/internal_trusted_storage/pal_internal_trusted_storage_intf.h
        psa-arch-tests/api-tests/dev_apis/storage/test_storage_common.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s001/test_s001.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s001/test_s001.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s001/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s001/test_entry_s001.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s002/test_s002.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s002/test_s002.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s002/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s002/test_entry_s002.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s003/test_s003.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s003/test_s003.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s003/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s003/test_entry_s003.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s004/test_s004.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s004/test_s004.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s004/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s004/test_entry_s004.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s005/test_s005.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s005/test_s005.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s005/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s005/test_entry_s005.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s006/test_s006.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s006/test_s006.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s006/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s006/test_entry_s006.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s007/test_s007.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s007/test_s007.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s007/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s007/test_entry_s007.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s008/test_s008.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s008/test_s008.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s008/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s008/test_entry_s008.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s009/test_s009.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s009/test_s009.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s009/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s009/test_entry_s009.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s010/test_s010.c
        psa-arch-tests/api-tests/dev_apis/storage/test_s010/test_s010.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s010/test_data.h
        psa-arch-tests/api-tests/dev_apis/storage/test_s010/test_entry_s010.c
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
    mcux_add_include(
        INCLUDES
        psa-arch-tests/api-tests/platform/targets/common/nspe/internal_trusted_storage
        psa-arch-tests/api-tests/dev_apis/storage
        psa-arch-tests/api-tests/dev_apis/storage/test_s001
        psa-arch-tests/api-tests/dev_apis/storage/test_s002
        psa-arch-tests/api-tests/dev_apis/storage/test_s003
        psa-arch-tests/api-tests/dev_apis/storage/test_s004
        psa-arch-tests/api-tests/dev_apis/storage/test_s005
        psa-arch-tests/api-tests/dev_apis/storage/test_s006
        psa-arch-tests/api-tests/dev_apis/storage/test_s007
        psa-arch-tests/api-tests/dev_apis/storage/test_s008
        psa-arch-tests/api-tests/dev_apis/storage/test_s009
        psa-arch-tests/api-tests/dev_apis/storage/test_s010
        BASE_PATH ${SdkRootDirPath}/middleware/tfm/
    )
endif()

