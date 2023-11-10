/*
 * Copyright (c) 2022 Arm Limited. All rights reserved.
 * Copyright 2023-2024 NXP
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

#include "tfm_hal_device_header.h"

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
extern uint64_t __STACK_SEAL;
#endif

typedef void(*VECTOR_TABLE_Type)(void);

extern void __PROGRAM_START(void) __NO_RETURN;

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void Reset_Handler  (void) __NO_RETURN;

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

DEFAULT_IRQ_HANDLER(DefaultISR)

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

/* Driver Exceptions */
DEFAULT_DRIVER_IRQ_HANDLER(WDT0_IRQHandler, WDT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_IRQHandler, DMA0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO_INTA_IRQHandler, GPIO_INTA_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO_INTB_IRQHandler, GPIO_INTB_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT0_IRQHandler, PIN_INT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT1_IRQHandler, PIN_INT1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT2_IRQHandler, PIN_INT2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT3_IRQHandler, PIN_INT3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(UTICK_IRQHandler, UTICK_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MRT0_IRQHandler, MRT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER0_IRQHandler, CTIMER0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER1_IRQHandler, CTIMER1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SCT0_IRQHandler, SCT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER3_IRQHandler, CTIMER3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLEXCOMM0_IRQHandler, FLEXCOMM0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLEXCOMM1_IRQHandler, FLEXCOMM1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLEXCOMM2_IRQHandler, FLEXCOMM2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLEXCOMM3_IRQHandler, FLEXCOMM3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved34_IRQHandler, Reserved34_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved35_IRQHandler, Reserved35_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLEXCOMM14_IRQHandler, FLEXCOMM14_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved37_IRQHandler, Reserved37_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved38_IRQHandler, Reserved38_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MRT1_IRQHandler, MRT1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved40_IRQHandler, Reserved40_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMIC0_IRQHandler, DMIC0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WFD_IRQHandler, WFD_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(HYPERVISOR_IRQHandler, HYPERVISOR_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SECUREVIOLATION_IRQHandler, SECUREVIOLATION_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(HWVAD0_IRQHandler, HWVAD0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved46_IRQHandler, Reserved46_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved47_IRQHandler, Reserved47_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RTC_IRQHandler, RTC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved49_IRQHandler, Reserved49_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved50_IRQHandler, Reserved50_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT4_IRQHandler, PIN_INT4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT5_IRQHandler, PIN_INT5_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT6_IRQHandler, PIN_INT6_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT7_IRQHandler, PIN_INT7_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER2_IRQHandler, CTIMER2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER4_IRQHandler, CTIMER4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(OS_EVENT_IRQHandler, OS_EVENT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLEXSPI_IRQHandler, FLEXSPI_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved59_IRQHandler, Reserved59_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved60_IRQHandler, Reserved60_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved61_IRQHandler, Reserved61_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SDU_IRQHandler, SDU_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SGPIO_INTA_IRQHandler, SGPIO_INTA_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SGPIO_INTB_IRQHandler, SGPIO_INTB_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved65_IRQHandler, Reserved65_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(USB_IRQHandler, USB_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved67_IRQHandler, Reserved67_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved68_IRQHandler, Reserved68_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved69_IRQHandler, Reserved69_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA1_IRQHandler, DMA1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PUF_IRQHandler, PUF_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(POWERQUAD_IRQHandler, POWERQUAD_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved73_IRQHandler, Reserved73_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved74_IRQHandler, Reserved74_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved75_IRQHandler, Reserved75_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved76_IRQHandler, Reserved76_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LCDIC_IRQHandler, LCDIC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CAPT_PULSE_IRQHandler, CAPT_PULSE_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved79_IRQHandler, Reserved79_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_WAKEUP_DONE0_IRQHandler, WL_MCI_WAKEUP_DONE0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_WAKEUP_DONE1_IRQHandler, WL_MCI_WAKEUP_DONE1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_WAKEUP_DONE2_IRQHandler, WL_MCI_WAKEUP_DONE2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_WAKEUP_DONE3_IRQHandler, WL_MCI_WAKEUP_DONE3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_WAKEUP_DONE4_IRQHandler, WL_MCI_WAKEUP_DONE4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_WAKEUP_DONE5_IRQHandler, WL_MCI_WAKEUP_DONE5_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_WAKEUP_DONE6_IRQHandler, WL_MCI_WAKEUP_DONE6_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_WAKEUP_DONE7_IRQHandler, WL_MCI_WAKEUP_DONE7_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_WAKEUP0_IRQHandler, WL_MCI_WAKEUP0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_WAKEUP1_IRQHandler, WL_MCI_WAKEUP1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_INT0_IRQHandler, WL_MCI_INT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_INT1_IRQHandler, WL_MCI_INT1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_INT2_IRQHandler, WL_MCI_INT2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_INT3_IRQHandler, WL_MCI_INT3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_INT4_IRQHandler, WL_MCI_INT4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_INT5_IRQHandler, WL_MCI_INT5_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_INT6_IRQHandler, WL_MCI_INT6_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_MCI_INT7_IRQHandler, WL_MCI_INT7_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_WAKEUP_DONE0_IRQHandler, BLE_MCI_WAKEUP_DONE0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_WAKEUP_DONE1_IRQHandler, BLE_MCI_WAKEUP_DONE1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_WAKEUP_DONE2_IRQHandler, BLE_MCI_WAKEUP_DONE2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_WAKEUP_DONE3_IRQHandler, BLE_MCI_WAKEUP_DONE3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_WAKEUP_DONE4_IRQHandler, BLE_MCI_WAKEUP_DONE4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_WAKEUP_DONE5_IRQHandler, BLE_MCI_WAKEUP_DONE5_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_WAKEUP_DONE6_IRQHandler, BLE_MCI_WAKEUP_DONE6_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_WAKEUP_DONE7_IRQHandler, BLE_MCI_WAKEUP_DONE7_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_WAKEUP0_IRQHandler, BLE_MCI_WAKEUP0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_WAKEUP1_IRQHandler, BLE_MCI_WAKEUP1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_INT0_IRQHandler, BLE_MCI_INT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_INT1_IRQHandler, BLE_MCI_INT1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_INT2_IRQHandler, BLE_MCI_INT2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_INT3_IRQHandler, BLE_MCI_INT3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_INT4_IRQHandler, BLE_MCI_INT4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_INT5_IRQHandler, BLE_MCI_INT5_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_INT6_IRQHandler, BLE_MCI_INT6_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_MCI_INT7_IRQHandler, BLE_MCI_INT7_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN0_INT_IRQHandler, PIN0_INT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN1_INT_IRQHandler, PIN1_INT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ELS_IRQHandler, ELS_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ELS_GDET_IRQ_IRQHandler, ELS_GDET_IRQ_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ELS_GDET_ERR_IRQHandler, ELS_GDET_ERR_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PKC_IRQHandler, PKC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PKC_ERR_IRQHandler, PKC_ERR_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CDOG_IRQHandler, CDOG_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GAU_GPDAC_INT_FUNC11_IRQHandler, GAU_GPDAC_INT_FUNC11_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GAU_ACOMP_INT_WKUP11_IRQHandler, GAU_ACOMP_INT_WKUP11_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GAU_ACOMP_INT_FUNC11_IRQHandler, GAU_ACOMP_INT_FUNC11_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GAU_GPADC1_INT_FUNC11_IRQHandler, GAU_GPADC1_INT_FUNC11_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GAU_GPADC0_INT_FUNC11_IRQHandler, GAU_GPADC0_INT_FUNC11_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(USIM_IRQHandler, USIM_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(OCOTP_IRQHandler, OCOTP_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ENET_IRQHandler, ENET_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ENET_TIMER_IRQHandler, ENET_TIMER_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BOD_1_85_INT_IRQHandler, BOD_1_85_INT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BOD_1_85_NEG_IRQHandler, BOD_1_85_NEG_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ITRC_IRQHandler, ITRC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BTU_HOST_TRIGGER0_IRQHandler, BTU_HOST_TRIGGER0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BTU_HOST_TRIGGER1_IRQHandler, BTU_HOST_TRIGGER1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BTU_HOST_TRIGGER2_IRQHandler, BTU_HOST_TRIGGER2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(TRNG_IRQHandler, TRNG_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(AHB_MEM_ACC_CHECKER_VIO_INT_C_OR_IRQHandler, AHB_MEM_ACC_CHECKER_VIO_INT_C_OR_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(AHB_MEM_ACC_CHECKER_VIO_INT_S_OR_IRQHandler, AHB_MEM_ACC_CHECKER_VIO_INT_S_OR_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WL_ACC_INT_IRQHandler, WL_ACC_INT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(BLE_ACC_INT_IRQHandler, BLE_ACC_INT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GDMA_IRQHandler, GDMA_DriverIRQHandler)

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const VECTOR_TABLE_Type __VECTOR_TABLE[];
       const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),            /* Initial Stack Pointer */
  Reset_Handler,                                 /* Reset Handler */
  NMI_Handler,                                   /* NMI Handler*/
  HardFault_Handler,                             /* Hard Fault Handler*/
  MemManage_Handler,                             /* MPU Fault Handler*/
  BusFault_Handler,                              /* Bus Fault Handler*/
  UsageFault_Handler,                            /* Usage Fault Handler*/
  SecureFault_Handler,                           /* Secure Fault Handler */
  0,                                             /* Reserved*/
  0,                                             /* Reserved*/
  0,                                             /* Reserved*/
  SVC_Handler,                                   /* SVCall Handler*/
  DebugMon_Handler,                              /* Debug Monitor Handler*/
  0,                                             /* Reserved*/
  PendSV_Handler,                                /* PendSV Handler*/
  SysTick_Handler,                               /* SysTick Handler*/


  /* External Interrupts */
  WDT0_IRQHandler,                                /* Windowed watchdog timer 0 (CM33 watchdog) */
  DMA0_IRQHandler,                                /* DMA controller 0 (secure or CM33 DMA) */
  GPIO_INTA_IRQHandler,                           /* GPIO interrupt A */
  GPIO_INTB_IRQHandler,                           /* GPIO interrupt B */
  PIN_INT0_IRQHandler,                            /* Pin interrupt 0 or pattern match engine slice 0 int */
  PIN_INT1_IRQHandler,                            /* Pin interrupt 1 or pattern match engine slice 1 int */
  PIN_INT2_IRQHandler,                            /* Pin interrupt 2 or pattern match engine slice 2 int */
  PIN_INT3_IRQHandler,                            /* Pin interrupt 3 or pattern match engine slice 3 int */
  UTICK_IRQHandler,                               /* Micro-tick Timer */
  MRT0_IRQHandler,                                /* Multi-Rate Timer. Global MRT interrupts */
  CTIMER0_IRQHandler,                             /* Standard counter/timer CTIMER0 */
  CTIMER1_IRQHandler,                             /* Standard counter/timer CTIMER1 */
  SCT0_IRQHandler,                                /* SCTimer/PWM */
  CTIMER3_IRQHandler,                             /* Standard counter/timer CTIMER3 */
  FLEXCOMM0_IRQHandler,                           /* Flexcomm Interface 0 (USART, SPI, I2C, I2S) */
  FLEXCOMM1_IRQHandler,                           /* Flexcomm Interface 1 (USART, SPI, I2C, I2S) */
  FLEXCOMM2_IRQHandler,                           /* Flexcomm Interface 2 (USART, SPI, I2C, I2S) */
  FLEXCOMM3_IRQHandler,                           /* Flexcomm Interface 3 (USART, SPI, I2C, I2S) */
  Reserved34_IRQHandler,                          /* xxx Interrupt 34 */
  Reserved35_IRQHandler,                          /* xxx Interrupt 35 */
  FLEXCOMM14_IRQHandler,                          /* Flexcomm Interface 14 (USART, SPI, I2C, I2S) */
  Reserved37_IRQHandler,                          /* xxx Interrupt 37 */
  Reserved38_IRQHandler,                          /* xxx Interrupt 38 */
  MRT1_IRQHandler,                                /* Free Multi-rate timer. Global MRT interrupts */
  Reserved40_IRQHandler,                          /* xxx Interrupt 40 */
  DMIC0_IRQHandler,                               /* Digital microphone and DMIC subsystem */
  WFD_IRQHandler,                                 /* Wakeup From Deepsleep */
  HYPERVISOR_IRQHandler,                          /* Hypervisor service software interrupt */
  SECUREVIOLATION_IRQHandler,                     /* Secure violation */
  HWVAD0_IRQHandler,                              /* Hardware Voice Activity Detector */
  Reserved46_IRQHandler,                          /* xxx Interrupt 46 */
  Reserved47_IRQHandler,                          /* xxx Interrupt 47 */
  RTC_IRQHandler,                                 /* RTC alarm and wake-up */
  Reserved49_IRQHandler,                          /* xxx Interrupt 49 */
  Reserved50_IRQHandler,                          /* xxx Interrupt 50 */
  PIN_INT4_IRQHandler,                            /* Pin interrupt 4 or pattern match engine slice 4 int */
  PIN_INT5_IRQHandler,                            /* Pin interrupt 5 or pattern match engine slice 5 int */
  PIN_INT6_IRQHandler,                            /* Pin interrupt 6 or pattern match engine slice 6 int */
  PIN_INT7_IRQHandler,                            /* Pin interrupt 7 or pattern match engine slice 7 int */
  CTIMER2_IRQHandler,                             /* Standard counter/timer CTIMER2 */
  CTIMER4_IRQHandler,                             /* Standard counter/timer CTIMER4 */
  OS_EVENT_IRQHandler,                            /* OS event timer */
  FLEXSPI_IRQHandler,                             /* FLEXSPI interface */
  Reserved59_IRQHandler,                          /* xxx Interrupt 59 */
  Reserved60_IRQHandler,                          /* xxx Interrupt 60 */
  Reserved61_IRQHandler,                          /* xxx Interrupt 61 */
  SDU_IRQHandler,                                 /* SDIO */
  SGPIO_INTA_IRQHandler,                          /* Secure GPIO interrupt A */
  SGPIO_INTB_IRQHandler,                          /* Secure GPIO interrupt B */
  Reserved65_IRQHandler,                          /* xxx Interrupt 65 */
  USB_IRQHandler,                                 /* High-speed USB device/host */
  Reserved67_IRQHandler,                          /* xxx Interrupt 67 */
  Reserved68_IRQHandler,                          /* xxx Interrupt 68 */
  Reserved69_IRQHandler,                          /* xxx Interrupt 69 */
  DMA1_IRQHandler,                                /* DMA controller 1 (non-secure or HiFi 4 DMA) */
  PUF_IRQHandler,                                 /* Physical Unclonable Function */
  POWERQUAD_IRQHandler,                           /* PowerQuad math coprocessor */
  Reserved73_IRQHandler,                          /* xxx Interrupt 73 */
  Reserved74_IRQHandler,                          /* xxx Interrupt 74 */
  Reserved75_IRQHandler,                          /* xxx Interrupt 75 */
  Reserved76_IRQHandler,                          /* xxx Interrupt 76 */
  LCDIC_IRQHandler,                               /* LCDIC */
  CAPT_PULSE_IRQHandler,                          /* Capture timer */
  Reserved79_IRQHandler,                          /* xxx Interrupt 79 */
  WL_MCI_WAKEUP_DONE0_IRQHandler,                 /* WL to MCI, Wakeup done 0 */
  WL_MCI_WAKEUP_DONE1_IRQHandler,                 /* WL to MCI, Wakeup done 1 */
  WL_MCI_WAKEUP_DONE2_IRQHandler,                 /* WL to MCI, Wakeup done 2 */
  WL_MCI_WAKEUP_DONE3_IRQHandler,                 /* WL to MCI, Wakeup done 3 */
  WL_MCI_WAKEUP_DONE4_IRQHandler,                 /* WL to MCI, Wakeup done 4 */
  WL_MCI_WAKEUP_DONE5_IRQHandler,                 /* WL to MCI, Wakeup done 5 */
  WL_MCI_WAKEUP_DONE6_IRQHandler,                 /* WL to MCI, Wakeup done 6 */
  WL_MCI_WAKEUP_DONE7_IRQHandler,                 /* WL to MCI, Wakeup done 7 */
  WL_MCI_WAKEUP0_IRQHandler,                      /* IMU_INT0: Cpu1_to_cpu3_msg_rdy_imu wl_mci_wakeup[0] */
  WL_MCI_WAKEUP1_IRQHandler,                      /* GP_INT from WL */
  WL_MCI_INT0_IRQHandler,                         /* IMU_INT: Imu13_cpu3_msg_space_avail */
  WL_MCI_INT1_IRQHandler,                         /* reserved */
  WL_MCI_INT2_IRQHandler,                         /* reserved */
  WL_MCI_INT3_IRQHandler,                         /* reserved */
  WL_MCI_INT4_IRQHandler,                         /* reserved */
  WL_MCI_INT5_IRQHandler,                         /* reserved */
  WL_MCI_INT6_IRQHandler,                         /* reserved */
  WL_MCI_INT7_IRQHandler,                         /* reserved */
  BLE_MCI_WAKEUP_DONE0_IRQHandler,                /* BLE to MCI, Wakeup done 0 */
  BLE_MCI_WAKEUP_DONE1_IRQHandler,                /* BLE to MCI, Wakeup done 1 */
  BLE_MCI_WAKEUP_DONE2_IRQHandler,                /* BLE to MCI, Wakeup done 2 */
  BLE_MCI_WAKEUP_DONE3_IRQHandler,                /* BLE to MCI, Wakeup done 3 */
  BLE_MCI_WAKEUP_DONE4_IRQHandler,                /* BLE to MCI, Wakeup done 4 */
  BLE_MCI_WAKEUP_DONE5_IRQHandler,                /* BLE to MCI, Wakeup done 5 */
  BLE_MCI_WAKEUP_DONE6_IRQHandler,                /* BLE to MCI, Wakeup done 6 */
  BLE_MCI_WAKEUP_DONE7_IRQHandler,                /* BLE to MCI, Wakeup done 7 */
  BLE_MCI_WAKEUP0_IRQHandler,                     /* IMU_INT0: Cpu2_to_cpu3_msg_rdy_imu wl_mci_wakeup[0] */
  BLE_MCI_WAKEUP1_IRQHandler,                     /* GP_INT from BLE */
  BLE_MCI_INT0_IRQHandler,                        /* IMU_INT: Imu13_cpu3_msg_space_avail */
  BLE_MCI_INT1_IRQHandler,                        /* reserved */
  BLE_MCI_INT2_IRQHandler,                        /* reserved */
  BLE_MCI_INT3_IRQHandler,                        /* reserved */
  BLE_MCI_INT4_IRQHandler,                        /* reserved */
  BLE_MCI_INT5_IRQHandler,                        /* reserved */
  BLE_MCI_INT6_IRQHandler,                        /* reserved */
  BLE_MCI_INT7_IRQHandler,                        /* reserved */
  PIN0_INT_IRQHandler,                            /* From AON GPIO */
  PIN1_INT_IRQHandler,                            /* From AON GPIO */
  ELS_IRQHandler,                                 /* ELS */
  ELS_GDET_IRQ_IRQHandler,                        /* ELS IRQ line for GDET error */
  ELS_GDET_ERR_IRQHandler,                        /* ELS Ungated latched error */
  PKC_IRQHandler,                                 /* PKC interrupt */
  PKC_ERR_IRQHandler,                             /* PKC error */
  CDOG_IRQHandler,                                /* Code watch dog timmer */
  GAU_GPDAC_INT_FUNC11_IRQHandler,                /* GAU */
  GAU_ACOMP_INT_WKUP11_IRQHandler,                /* GAU */
  GAU_ACOMP_INT_FUNC11_IRQHandler,                /* GAU */
  GAU_GPADC1_INT_FUNC11_IRQHandler,               /* GAU */
  GAU_GPADC0_INT_FUNC11_IRQHandler,               /* GAU */
  USIM_IRQHandler,                                /* USIM */
  OCOTP_IRQHandler,                               /* OTP */
  ENET_IRQHandler,                                /* ENET */
  ENET_TIMER_IRQHandler,                          /* ENET */
  BOD_1_85_INT_IRQHandler,                        /* PMIP */
  BOD_1_85_NEG_IRQHandler,                        /* Bod_1_85_int negedge */
  ITRC_IRQHandler,                                /* ITRC */
  BTU_HOST_TRIGGER0_IRQHandler,                   /* Btu host trigger0 */
  BTU_HOST_TRIGGER1_IRQHandler,                   /* Btu host trigger1 */
  BTU_HOST_TRIGGER2_IRQHandler,                   /* Btu host trigger2 */
  TRNG_IRQHandler,                                /* TRNG */
  AHB_MEM_ACC_CHECKER_VIO_INT_C_OR_IRQHandler,    /* ahb memory access checker - CM33 code bus */
  AHB_MEM_ACC_CHECKER_VIO_INT_S_OR_IRQHandler,    /* ahb memory access checker - CM33 sys bus */
  WL_ACC_INT_IRQHandler,                          /* Cpu access wlan when wlan is powered off */
  BLE_ACC_INT_IRQHandler,                         /* Cpu access wlan when ble is powered off */
  GDMA_IRQHandler,                                /* GDMA */
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
