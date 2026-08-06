# NXP Platform IOCTL Services for TF-M

## Overview

The NXP Platform IOCTL Services provide a secure gateway that allows Non-Secure (NS) world
applications to perform flash operations and ROM API calls through the TF-M Platform partition.
All operations are dispatched via `tfm_platform_ioctl()` and execute entirely in the Secure (S)
world, ensuring that NS code never has direct write access to flash.

Currently supported target: **FRDM-MCXL255**.

---

## Feature Description

### NS Storage (NXP_NS_STORAGE) and ROM API TFM Interface (MCUX_ROMAPI_TFM_INTERFACE)

When `NXP_NS_STORAGE` is defined, a dedicated region of internal flash is reserved for
Non-Secure application use. The S-world exposes a set of IOCTL operations so the NS world
can erase, program, verify, and read that region without requiring direct flash access.

When `MCUX_ROMAPI_TFM_INTERFACE` is defined (in addition to `NXP_NS_STORAGE`), two additional
ROM API operations are exposed to the NS world.

Available IOCTL operations (defined in `tfm_ioctl_api.h`):

| Request ID | Operation |
|---|---|
| `TFM_PLATFORM_IOCTL_FLASH_ERASE_SECTOR` | Erase one or more 8 KB sectors |
| `TFM_PLATFORM_IOCTL_FLASH_PROGRAM` | Program data (16-byte phrase aligned) |
| `TFM_PLATFORM_IOCTL_FLASH_VERIFY_PROGRAM` | Verify programmed data |
| `TFM_PLATFORM_IOCTL_FLASH_VERIFY_ERASE_PHRASE` | Verify 16-byte phrases are erased |
| `TFM_PLATFORM_IOCTL_FLASH_VERIFY_ERASE_PAGE` | Verify 128-byte pages are erased |
| `TFM_PLATFORM_IOCTL_FLASH_VERIFY_ERASE_SECTOR` | Verify 8 KB sectors are erased |
| `TFM_PLATFORM_IOCTL_FLASH_GET_PROPERTY` | Read a flash property value |
| `TFM_PLATFORM_IOCTL_FLASH_READ` | Read flash bytes into NS buffer |
| `TFM_PLATFORM_IOCTL_ROMAPI_GET_VERSION` | Read the ROM API version word |
| `TFM_PLATFORM_IOCTL_ROMAPI_RUN_BOOTLOADER` | Invoke the ROM bootloader |

---

## Source Files

| File | Side | Description |
|---|---|---|
| `frdmmcxl255/services/src/tfm_flash_ioctl_hal.c` | Secure | S-world HAL handler; receives IOCTL requests and calls the ROM flash driver |
| `frdmmcxl255/services/src/tfm_ioctl_ns_api.c` | Non-Secure | NS-world API wrappers; calls `tfm_platform_ioctl()` for each operation |
| `frdmmcxl255/services/include/tfm_ioctl_api.h` | Both | Request IDs, argument structs, and NS-callable function declarations |
| `frdmmcxl255/services/include/fsl_flash_ns_proxy.h` | Non-Secure | Flash NS proxy interface |

---

## How to Enable

### 1. Kconfig (prj.conf)

Enable the Kconfig options for the side(s) you need:

**Secure side only** (S-world flash HAL, defines `-DNXP_NS_STORAGE`):
```
CONFIG_MCUX_COMPONENT_middleware.tfm.s.romapi=y
```

**Non-Secure side** (NS-world API wrappers, defines `-DNXP_NS_STORAGE` and
`-DMCUX_ROMAPI_TFM_INTERFACE`):
```
CONFIG_MCUX_COMPONENT_middleware.tfm.ns.romapi=y
```

Both options are found in the **Platform IOCTL Services** menu inside the TF-M Kconfig menu
tree. Each option automatically selects `driver.romapi` so the ROM API driver is pulled in.

### 2. What the options enable

| Kconfig option | Preprocessor macros | Effect |
|---|---|---|
| `middleware.tfm.s.romapi` | `-DNXP_NS_STORAGE` | Compiles `tfm_flash_ioctl_hal.c` into the S image; enables the flash storage region |
| `middleware.tfm.ns.romapi` | `-DNXP_NS_STORAGE` `-DMCUX_ROMAPI_TFM_INTERFACE` | Compiles `tfm_ioctl_ns_api.c` into the NS image; enables the ROM API interface |

### 3. Typical dual-image project

A project that needs both S and NS sides should set:
```
CONFIG_MCUX_COMPONENT_middleware.tfm.s.romapi=y
CONFIG_MCUX_COMPONENT_middleware.tfm.ns.romapi=y
```

---

## Security Notes

- The S-world HAL validates all NS pointers using CMSE checks before accessing them.
- The NS world has no direct write path to flash; all mutations go through the S-world
  `tfm_platform_ioctl()` gate.
- The `NXP_NS_STORAGE` region boundaries are enforced on the S side; requests outside the
  designated region are rejected.
