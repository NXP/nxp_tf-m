/*
 * Copyright (c) 2022 Arm Limited. All rights reserved.
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file is derivative of CMSIS V5.9.0 startup_ARMCM33.c
 * Git SHA: 2b7495b8535bdcb306dac29b9ded4cfb679d7e5c
 */

/* NS linker scripts using the default CMSIS style naming conventions, while the
 * secure and bl2 linker scripts remain untouched (region.h compatibility).
 * To be compatible with the untouched files (which using ARMCLANG naming style),
 * we have to override __INITIAL_SP and __STACK_LIMIT labels. */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) 
#include "cmsis_override.h"
#endif

#include "cmsis.h"

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
extern uint64_t __STACK_SEAL;
#endif

typedef void(*VECTOR_TABLE_Type)(void);

extern __NO_RETURN void __PROGRAM_START(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/

void default_handler (void);
void default_handler (void) {
    while(1);
}

#define DEFAULT_IRQ_HANDLER(handler_name)  \
void handler_name (void) __attribute__((weak, alias("default_handler")));

#define DEFAULT_DRIVER_IRQ_HANDLER(handler_name, driver_handler_name)  \
DEFAULT_IRQ_HANDLER(driver_handler_name) \
void __WEAK handler_name (void); \
void handler_name (void) { \
    driver_handler_name(); \
}

/* Exceptions */
DEFAULT_IRQ_HANDLER(NMI_Handler)
DEFAULT_IRQ_HANDLER(HardFault_Handler)
DEFAULT_IRQ_HANDLER(MemManage_Handler)
DEFAULT_IRQ_HANDLER(BusFault_Handler)
DEFAULT_IRQ_HANDLER(UsageFault_Handler)
DEFAULT_IRQ_HANDLER(SecureFault_Handler)
DEFAULT_IRQ_HANDLER(SVC_Handler)
DEFAULT_IRQ_HANDLER(DebugMon_Handler)
DEFAULT_IRQ_HANDLER(PendSV_Handler)
DEFAULT_IRQ_HANDLER(SysTick_Handler)

DEFAULT_DRIVER_IRQ_HANDLER(WDT_BOD_IRQHandler, WDT_BOD_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_IRQHandler, DMA0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GINT0_IRQHandler, GINT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved19_IRQHandler, Reserved19_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT0_IRQHandler, PIN_INT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT1_IRQHandler, PIN_INT1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT2_IRQHandler, PIN_INT2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT3_IRQHandler, PIN_INT3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(UTICK0_IRQHandler, UTICK0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MRT0_IRQHandler, MRT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER0_IRQHandler, CTIMER0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER1_IRQHandler, CTIMER1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SCT0_IRQHandler, SCT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER3_IRQHandler, CTIMER3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLEXCOMM0_IRQHandler, FLEXCOMM0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLEXCOMM1_IRQHandler, FLEXCOMM1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLEXCOMM2_IRQHandler, FLEXCOMM2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved33_IRQHandler, Reserved33_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved34_IRQHandler, Reserved34_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved35_IRQHandler, Reserved35_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved36_IRQHandler, Reserved36_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved37_IRQHandler, Reserved37_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_LL_IRQHandler, BLE_LL_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_SLP_TMR_IRQHandler, BLE_SLP_TMR_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WDT_IRQHandler, WDT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved41_IRQHandler, Reserved41_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved42_IRQHandler, Reserved42_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BOD1_IRQHandler, BOD1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BOD2_IRQHandler, BOD2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RTC_IRQHandler, RTC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved46_IRQHandler, Reserved46_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WAKE_DSLP_IRQHandler, WAKE_DSLP_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT4_IRQHandler, PIN_INT4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT5_IRQHandler, PIN_INT5_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT6_IRQHandler, PIN_INT6_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT7_IRQHandler, PIN_INT7_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER2_IRQHandler, CTIMER2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER4_IRQHandler, CTIMER4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(OS_EVENT_IRQHandler, OS_EVENT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SPIFI_IRQHandler, SPIFI_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved56_IRQHandler, Reserved56_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved57_IRQHandler, Reserved57_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved58_IRQHandler, Reserved58_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLASH_CCIF_IRQHandler, FLASH_CCIF_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLASH_DFIF_IRQHandler, FLASH_DFIF_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved61_IRQHandler, Reserved61_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved62_IRQHandler, Reserved62_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved63_IRQHandler, Reserved63_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved64_IRQHandler, Reserved64_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SEC_HYPERVISOR_CALL_IRQHandler, SEC_HYPERVISOR_CALL_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SEC_GPIO_INT0_IRQ0_IRQHandler, SEC_GPIO_INT0_IRQ0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SEC_GPIO_INT0_IRQ1_IRQHandler, SEC_GPIO_INT0_IRQ1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PLU_IRQHandler, PLU_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SEC_VIO_IRQHandler, SEC_VIO_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(HASHCRYPT_IRQHandler, HASHCRYPT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CASPER_IRQHandler, CASPER_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PUF_IRQHandler, PUF_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(TRNG_IRQHandler, TRNG_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA1_IRQHandler, DMA1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved75_IRQHandler, Reserved75_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CDOG_IRQHandler, CDOG_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved77_IRQHandler, Reserved77_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved78_IRQHandler, Reserved78_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WAKE_PAD_IRQHandler, WAKE_PAD_DriverIRQHandler)
/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const VECTOR_TABLE_Type __VECTOR_TABLE[];
       const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),            /*      Initial Stack Pointer */
  Reset_Handler,                     /* Reset Handler */
  NMI_Handler,                       /* NMI Handler*/
  HardFault_Handler,                 /* Hard Fault Handler*/
  MemManage_Handler,                 /* MPU Fault Handler*/
  BusFault_Handler,                  /* Bus Fault Handler*/
  UsageFault_Handler,                /* Usage Fault Handler*/
  SecureFault_Handler,               /* Secure Fault Handler */
  0,                                 /* Reserved*/
  0,                                 /* Reserved*/
  0,                                 /* Reserved*/
  SVC_Handler,                       /* SVCall Handler*/
  DebugMon_Handler,                  /* Debug Monitor Handler*/
  0,                                 /* Reserved*/
  PendSV_Handler,                    /* PendSV Handler*/
  SysTick_Handler,                   /* SysTick Handler*/

/* External Interrupts*/
  WDT_BOD_IRQHandler,                /* Windowed watchdog timer, Brownout detect, Flash interrupt */
  DMA0_IRQHandler,                   /* DMA0 controller */
  GINT0_IRQHandler,                  /* GPIO group 0 */
  Reserved19_IRQHandler,             /* Reserved interrupt*/
  PIN_INT0_IRQHandler,               /* Pin interrupt 0 or pattern match engine slice 0 */
  PIN_INT1_IRQHandler,               /* Pin interrupt 1or pattern match engine slice 1 */
  PIN_INT2_IRQHandler,               /* Pin interrupt 2 or pattern match engine slice 2 */
  PIN_INT3_IRQHandler,               /* Pin interrupt 3 or pattern match engine slice 3 */
  UTICK0_IRQHandler,                 /* Micro-tick Timer */
  MRT0_IRQHandler,                   /* Multi-rate timer */
  CTIMER0_IRQHandler,                /* Standard counter/timer CTIMER0 */
  CTIMER1_IRQHandler,                /* Standard counter/timer CTIMER1 */
  SCT0_IRQHandler,                   /* SCTimer/PWM */
  CTIMER3_IRQHandler,                /* Standard counter/timer CTIMER3 */
  FLEXCOMM0_IRQHandler,              /* Flexcomm Interface 0 (USART, SPI, I2C, I2S, FLEXCOMM) */
  FLEXCOMM1_IRQHandler,              /* Flexcomm Interface 1 (USART, SPI, I2C, I2S, FLEXCOMM) */
  FLEXCOMM2_IRQHandler,              /* Flexcomm Interface 2 (USART, SPI, I2C, I2S, FLEXCOMM) */
  Reserved33_IRQHandler,             /* Reserved interrupt */
  Reserved34_IRQHandler,             /* Reserved interrupt */
  Reserved35_IRQHandler,             /* Reserved interrupt */
  Reserved36_IRQHandler,             /* Reserved interrupt */
  Reserved37_IRQHandler,             /* Reserved interrupt */
  BLE_LL_IRQHandler,                 /* BLE Link Layer interrupt */
  BLE_SLP_TMR_IRQHandler,            /* BLE Sleep Timer interrupt */
  WDT_IRQHandler,                    /* Windowed Watchdog interrupt */
  Reserved41_IRQHandler,             /* Reserved interrupt */
  Reserved42_IRQHandler,             /* Reserved interrupt */
  BOD1_IRQHandler,                   /* Brown-out detector 1 Interrupt */
  BOD2_IRQHandler,                   /* Brown-out detector 2 Interrupt */
  RTC_IRQHandler,                    /* RTC alarm and wake-up interrupts */
  Reserved46_IRQHandler,             /* Reserved interrupt */
  WAKE_DSLP_IRQHandler,              /* wakeup from deepsleep interrupt */
  PIN_INT4_IRQHandler,               /* Pin interrupt 4 or pattern match engine slice 4 int */
  PIN_INT5_IRQHandler,               /* Pin interrupt 5 or pattern match engine slice 5 int */
  PIN_INT6_IRQHandler,               /* Pin interrupt 6 or pattern match engine slice 6 int */
  PIN_INT7_IRQHandler,               /* Pin interrupt 7 or pattern match engine slice 7 int */
  CTIMER2_IRQHandler,                /* Standard counter/timer CTIMER2 */
  CTIMER4_IRQHandler,                /* Standard counter/timer CTIMER4 */
  OS_EVENT_IRQHandler,               /* OSEVTIMER0 and OSEVTIMER0_WAKEUP interrupts */
  SPIFI_IRQHandler,                  /* Quad SPI flash interface interrupt */
  Reserved56_IRQHandler,             /* Reserved interrupt */
  Reserved57_IRQHandler,             /* Reserved interrupt */
  Reserved58_IRQHandler,             /* Reserved interrupt */
  FLASH_CCIF_IRQHandler,             /* Flash command complete interrupt */
  FLASH_DFIF_IRQHandler,             /* Flash ECC double-bit error detect interrupt */
  Reserved61_IRQHandler,             /* Reserved interrupt */
  Reserved62_IRQHandler,             /* Reserved interrupt */
  Reserved63_IRQHandler,             /* Reserved interrupt */
  Reserved64_IRQHandler,             /* Reserved interrupt */
  SEC_HYPERVISOR_CALL_IRQHandler,    /* SEC_HYPERVISOR_CALL interrupt */
  SEC_GPIO_INT0_IRQ0_IRQHandler,     /* SEC_GPIO_INT0_IRQ0 interrupt */
  SEC_GPIO_INT0_IRQ1_IRQHandler,     /* SEC_GPIO_INT0_IRQ1 interrupt */
  PLU_IRQHandler,                    /* PLU interrupt */
  SEC_VIO_IRQHandler,                /* SEC_VIO interrupt */
  HASHCRYPT_IRQHandler,              /* HASHCRYPT interrupt */
  CASPER_IRQHandler,                 /* CASPER interrupt */
  PUF_IRQHandler,                    /* PUF interrupt */
  TRNG_IRQHandler,                   /* TRNG interrupt */
  DMA1_IRQHandler,                   /* DMA1 interrupt */
  Reserved75_IRQHandler,             /* Reserved interrupt */
  CDOG_IRQHandler,                   /* CDOG interrupt */
  Reserved77_IRQHandler,             /* Reserved interrupt */
  Reserved78_IRQHandler,             /* Reserved interrupt */  
  WAKE_PAD_IRQHandler,               /* Wakeup from pin interrupt */
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    __disable_irq();
#endif
    __set_PSP((uint32_t)(&__INITIAL_SP));

    __set_MSPLIM((uint32_t)(&__STACK_LIMIT));
    __set_PSPLIM((uint32_t)(&__STACK_LIMIT));

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    __TZ_set_STACKSEAL_S((uint32_t *)(&__STACK_SEAL));
#endif

    SystemInit();                             /* CMSIS System Initialization */
    __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}
