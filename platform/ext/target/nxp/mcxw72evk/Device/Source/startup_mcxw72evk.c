/*
 * Copyright (c) 2022 Arm Limited. All rights reserved.
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

DEFAULT_DRIVER_IRQ_HANDLER(WDT0_IRQHandler, WDT0_DriverIRQHandler)


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

DEFAULT_DRIVER_IRQ_HANDLER(CTI_IRQHandler, CTI_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CMC0_IRQHandler, CMC0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH0_IRQHandler, DMA0_CH0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH1_IRQHandler, DMA0_CH1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH2_IRQHandler, DMA0_CH2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH3_IRQHandler, DMA0_CH3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH4_IRQHandler, DMA0_CH4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH5_IRQHandler, DMA0_CH5_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH6_IRQHandler, DMA0_CH6_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH7_IRQHandler, DMA0_CH7_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH8_IRQHandler, DMA0_CH8_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH9_IRQHandler, DMA0_CH9_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH10_IRQHandler, DMA0_CH10_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH11_IRQHandler, DMA0_CH11_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH12_IRQHandler, DMA0_CH12_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH13_IRQHandler, DMA0_CH13_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH14_IRQHandler, DMA0_CH14_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH15_IRQHandler, DMA0_CH15_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EWM0_IRQHandler, EWM0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MCM0_IRQHandler, MCM0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MSCM0_IRQHandler, MSCM0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SPC0_IRQHandler, SPC0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WUU0_IRQHandler, WUU0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WDOG0_IRQHandler, WDOG0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WDOG1_IRQHandler, WDOG1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SCG0_IRQHandler, SCG0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SFA0_IRQHandler, SFA0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FMU0_IRQHandler, FMU0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ELE_CMD_IRQHandler, ELE_CMD_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ELE_SECURE_IRQHandler, ELE_SECURE_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ELE_NONSECURE_IRQHandler, ELE_NONSECURE_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(TRDC0_IRQHandler, TRDC0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RTC_Alarm_IRQHandler, RTC_Alarm_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RTC_Seconds_IRQHandler, RTC_Seconds_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPTMR0_IRQHandler, LPTMR0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPTMR1_IRQHandler, LPTMR1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPIT0_IRQHandler, LPIT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(TPM0_IRQHandler, TPM0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(TPM1_IRQHandler, TPM1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPI2C0_IRQHandler, LPI2C0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPI2C1_IRQHandler, LPI2C1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(I3C0_IRQHandler, I3C0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPSPI0_IRQHandler, LPSPI0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPSPI1_IRQHandler, LPSPI1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPUART0_IRQHandler, LPUART0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPUART1_IRQHandler, LPUART1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLEXIO0_IRQHandler, FLEXIO0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CAN0_IRQHandler, CAN0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RF_IMU0_IRQHandler, RF_IMU0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RF_IMU1_IRQHandler, RF_IMU1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RF_NBU_IRQHandler, RF_NBU_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RF_FMU_IRQHandler, RF_FMU_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RF_WOR_IRQHandler, RF_WOR_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RF_802_15_4_IRQHandler, RF_802_15_4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RF_Generic_IRQHandler, RF_Generic_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RF_BRIC_IRQHandler, RF_BRIC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RF_LANT_SW_IRQHandler, RF_LANT_SW_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RFMC_IRQHandler, RFMC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DSB_IRQHandler, DSB_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIOA_INT0_IRQHandler, GPIOA_INT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIOA_INT1_IRQHandler, GPIOA_INT1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIOB_INT0_IRQHandler, GPIOB_INT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIOB_INT1_IRQHandler, GPIOB_INT1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIOC_INT0_IRQHandler, GPIOC_INT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIOC_INT1_IRQHandler, GPIOC_INT1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIOD_INT0_IRQHandler, GPIOD_INT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIOD_INT1_IRQHandler, GPIOD_INT1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PORTA_EFT_IRQHandler, PORTA_EFT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PORTB_EFT_IRQHandler, PORTB_EFT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PORTC_EFT_IRQHandler, PORTC_EFT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PORTD_EFT_IRQHandler, PORTD_EFT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ADC0_IRQHandler, ADC0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPCMP0_IRQHandler, LPCMP0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPCMP1_IRQHandler, LPCMP1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(VBAT_IRQHandler, VBAT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved91_IRQHandler, Reserved91_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CAN1_IRQHandler, CAN1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(DSP_IRQHandler, DSP_DriverIRQHandler)

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
  CTI_IRQHandler,                   /* Cross Trigger Interface interrupt*/
  CMC0_IRQHandler,                  /* Core Mode Controller interrupt*/
  DMA0_CH0_IRQHandler,              /* eDMA channel 0 error or transfer complete*/
  DMA0_CH1_IRQHandler,              /* eDMA channel 1 error or transfer complete*/
  DMA0_CH2_IRQHandler,              /* eDMA channel 2 error or transfer complete*/
  DMA0_CH3_IRQHandler,              /* eDMA channel 3 error or transfer complete*/
  DMA0_CH4_IRQHandler,              /* eDMA channel 4 error or transfer complete*/
  DMA0_CH5_IRQHandler,              /* eDMA channel 5 error or transfer complete*/
  DMA0_CH6_IRQHandler,              /* eDMA channel 6 error or transfer complete*/
  DMA0_CH7_IRQHandler,              /* eDMA channel 7 error or transfer complete*/
  DMA0_CH8_IRQHandler,              /* eDMA channel 8 error or transfer complete*/
  DMA0_CH9_IRQHandler,              /* eDMA channel 9 error or transfer complete*/
  DMA0_CH10_IRQHandler,             /* eDMA channel 10 error or transfer complete*/
  DMA0_CH11_IRQHandler,             /* eDMA channel 11 error or transfer complete*/
  DMA0_CH12_IRQHandler,             /* eDMA channel 12 error or transfer complete*/
  DMA0_CH13_IRQHandler,             /* eDMA channel 13 error or transfer complete*/
  DMA0_CH14_IRQHandler,             /* eDMA channel 14 error or transfer complete*/
  DMA0_CH15_IRQHandler,             /* eDMA channel 15 error or transfer complete*/
  EWM0_IRQHandler,                  /* External Watchdog Monitor 0 interrupt*/
  MCM0_IRQHandler,                  /* Miscellaneous Control Module interrupt*/
  MSCM0_IRQHandler,                 /* Miscellaneous System Control Module interrupt*/
  SPC0_IRQHandler,                  /* System Power Controller 0 interrupt*/
  WUU0_IRQHandler,                  /* Wake-Up Unit 0 interrupt*/
  WDOG0_IRQHandler,                 /* Watchdog Timer 0 interrupt*/
  WDOG1_IRQHandler,                 /* Watchdog Timer 1 interrupt*/
  SCG0_IRQHandler,                  /* System Clock Generator 0 interrupt*/
  SFA0_IRQHandler,                  /* Singal Frequency Analyzer 0 interrupt*/
  FMU0_IRQHandler,                  /* Flash Memory Unit 0 interrupt*/
  ELE_CMD_IRQHandler,               /* EdgeLock enclave command interface interrupt*/
  ELE_SECURE_IRQHandler,            /* EdgeLock enclave interrupt*/
  ELE_NONSECURE_IRQHandler,         /* EdgeLock enclave non-secure interrupt*/
  TRDC0_IRQHandler,                 /* Trusted Resource Domain Controller 0 interrupt*/
  RTC_Alarm_IRQHandler,             /* Real Time Clock 0 alarm interrupt*/
  RTC_Seconds_IRQHandler,           /* Real Time Clock 0 seconds interrupt*/
  LPTMR0_IRQHandler,                /* Low-Power Timer0 interrupt*/
  LPTMR1_IRQHandler,                /* Low-Power Timer1 interrupt*/
  LPIT0_IRQHandler,                 /* Low-Power Periodic Interrupt Timer 0 interrupt*/
  TPM0_IRQHandler,                  /* Timer / PWM Module 0 interrupt*/
  TPM1_IRQHandler,                  /* Timer / PWM Module 1 interrupt*/
  LPI2C0_IRQHandler,                /* Low-Power Inter Integrated Circuit 0 interrupt*/
  LPI2C1_IRQHandler,                /* Low-Power Inter Integrated Circuit 1 interrupt*/
  I3C0_IRQHandler,                  /* Improved Inter-Integrated Circuit 0 interrupt*/
  LPSPI0_IRQHandler,                /* Low-Power Serial Peripheral Interface 0 interrupt*/
  LPSPI1_IRQHandler,                /* Low-Power Serial Peripheral Interface 1 interrupt*/
  LPUART0_IRQHandler,               /* Low-Power Universal Asynchronous Receiver/Transmitter 0 interrupt*/
  LPUART1_IRQHandler,               /* Low-Power Universal Asynchronous Receiver/Transmitter 1 interrupt*/
  FLEXIO0_IRQHandler,               /* Flexible Input/Output 0 interrupt*/
  CAN0_IRQHandler,                  /* Controller Area Network 0 interrupt*/
  RF_IMU0_IRQHandler,               /* Radio IMU interrupt 0 (msg_rdy_imu)*/
  RF_IMU1_IRQHandler,               /* Radio IMU interrupt 1(msg_space_avail_imu)*/
  RF_NBU_IRQHandler,                /* Radio NBU timeout interrupt*/
  RF_FMU_IRQHandler,                /* Radio FMU interrupt*/
  RF_WOR_IRQHandler,                /* Radio WOR RX FAIL interrupt*/
  RF_802_15_4_IRQHandler,           /* Radio Frequency 2.4GHz - 802.15.4 Link Layer interrupt*/
  RF_Generic_IRQHandler,            /* Radio Frequency 2.4 GHz - Generic Link Layer interrupt*/
  RF_BRIC_IRQHandler,               /* Radio Frequency 2.4 GHz - BRIC interrupt*/
  RF_LANT_SW_IRQHandler,            /* Radio Transceiver - Radio LANT_SW interrupt*/
  RFMC_IRQHandler,                  /* RFMC interrupt*/
  DSB_IRQHandler,                   /* Data Stream Buffer interrupt*/
  GPIOA_INT0_IRQHandler,            /* General Purpose Input/Output A interrupt 0*/
  GPIOA_INT1_IRQHandler,            /* General Purpose Input/Output A interrupt 1*/
  GPIOB_INT0_IRQHandler,            /* General Purpose Input/Output B interrupt 0*/
  GPIOB_INT1_IRQHandler,            /* General Purpose Input/Output B interrupt 1*/
  GPIOC_INT0_IRQHandler,            /* General Purpose Input/Output C interrupt 0*/
  GPIOC_INT1_IRQHandler,            /* General Purpose Input/Output C interrupt 1*/
  GPIOD_INT0_IRQHandler,            /* General Purpose Input/Output D interrupt 0*/
  GPIOD_INT1_IRQHandler,            /* General Purpose Input/Output D interrupt 1*/
  PORTA_EFT_IRQHandler,             /* PortA EFT interrupt*/
  PORTB_EFT_IRQHandler,             /* PortB EFT interrupt*/
  PORTC_EFT_IRQHandler,             /* PortC EFT interrupt*/
  PORTD_EFT_IRQHandler,             /* PortD EFT interrupt*/
  ADC0_IRQHandler,                  /* Analog-to-Digital Converter 0 interrupt*/
  LPCMP0_IRQHandler,                /* Low-Power Comparator 0 interrupt*/
  LPCMP1_IRQHandler,                /* Low-Power Comparator 1 interrupt*/
  VBAT_IRQHandler,                  /* Smart Power Switch Domain interrupt*/
  Reserved91_IRQHandler,            /* Reserved interrupt*/
  CAN1_IRQHandler,                   /* Controller Area Network 1 interrupt*/
  DSP_IRQHandler,                    /* LCE interrupt*/
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
