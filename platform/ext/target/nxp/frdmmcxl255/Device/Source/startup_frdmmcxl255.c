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

DEFAULT_DRIVER_IRQ_HANDLER(Reserved16_IRQHandler, Reserved16_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(CMC_IRQHandler, CMC_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH0_IRQHandler, DMA0_CH0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH1_IRQHandler, DMA0_CH1_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH2_IRQHandler, DMA0_CH2_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(DMA0_CH3_IRQHandler, DMA0_CH3_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved22_IRQHandler, Reserved22_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved23_IRQHandler, Reserved23_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved24_IRQHandler, Reserved24_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved25_IRQHandler, Reserved25_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(ERM0_IRQHandler, ERM0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved27_IRQHandler, Reserved27_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(FMU0_IRQHandler, FMU0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(GLIKEY0_IRQHandler, GLIKEY0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(MBC0_IRQHandler, MBC0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(SCG0_IRQHandler, SCG0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved32_IRQHandler, Reserved32_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved33_IRQHandler, Reserved33_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(WUU0_IRQHandler, WUU0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved35_IRQHandler, Reserved35_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved36_IRQHandler, Reserved36_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved37_IRQHandler, Reserved37_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved38_IRQHandler, Reserved38_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved39_IRQHandler, Reserved39_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved40_IRQHandler, Reserved40_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved41_IRQHandler, Reserved41_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(LPI2C0_IRQHandler, LPI2C0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(LPI2C1_IRQHandler, LPI2C1_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(LPSPI0_IRQHandler, LPSPI0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(LPSPI1_IRQHandler, LPSPI1_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved46_IRQHandler, Reserved46_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(LPUART0_IRQHandler, LPUART0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(LPUART1_IRQHandler, LPUART1_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved49_IRQHandler, Reserved49_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved50_IRQHandler, Reserved50_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved51_IRQHandler, Reserved51_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved52_IRQHandler, Reserved52_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved53_IRQHandler, Reserved53_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(CDOG0_IRQHandler, CDOG0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER0_IRQHandler, CTIMER0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER1_IRQHandler, CTIMER1_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER2_IRQHandler, CTIMER2_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved58_IRQHandler, Reserved58_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved59_IRQHandler, Reserved59_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved60_IRQHandler, Reserved60_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved61_IRQHandler, Reserved61_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved62_IRQHandler, Reserved62_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved63_IRQHandler, Reserved63_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved64_IRQHandler, Reserved64_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved65_IRQHandler, Reserved65_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved66_IRQHandler, Reserved66_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved67_IRQHandler, Reserved67_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved68_IRQHandler, Reserved68_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved69_IRQHandler, Reserved69_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(FREQME0_IRQHandler, FREQME0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved71_IRQHandler, Reserved71_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved72_IRQHandler, Reserved72_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(OS_EVENT_IRQHandler, OS_EVENT_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved74_IRQHandler, Reserved74_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(UTICK0_IRQHandler, UTICK0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(WWDT0_IRQHandler, WWDT0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved77_IRQHandler, Reserved77_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(ADC0_IRQHandler, ADC0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved79_IRQHandler, Reserved79_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(CMP0_IRQHandler, CMP0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved81_IRQHandler, Reserved81_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved82_IRQHandler, Reserved82_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved83_IRQHandler, Reserved83_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved84_IRQHandler, Reserved84_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved85_IRQHandler, Reserved85_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved86_IRQHandler, Reserved86_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(GPIO10_IRQHandler, GPIO10_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(GPIO11_IRQHandler, GPIO11_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(GPIO20_IRQHandler, GPIO20_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(GPIO21_IRQHandler, GPIO21_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(GPIO30_IRQHandler, GPIO30_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(GPIO31_IRQHandler, GPIO31_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved93_IRQHandler, Reserved93_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved94_IRQHandler, Reserved94_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved95_IRQHandler, Reserved95_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(MU_A_TX_IRQHandler, MU_A_TX_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(MU_A_RX_IRQHandler, MU_A_RX_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(MU_A_INT_IRQHandler, MU_A_INT_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(CDOG1_IRQHandler, CDOG1_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved100_IRQHandler, Reserved100_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved101_IRQHandler, Reserved101_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved102_IRQHandler, Reserved102_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved103_IRQHandler, Reserved103_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved104_IRQHandler, Reserved104_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved105_IRQHandler, Reserved105_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved106_IRQHandler, Reserved106_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved107_IRQHandler, Reserved107_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved108_IRQHandler, Reserved108_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved109_IRQHandler, Reserved109_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved110_IRQHandler, Reserved110_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved111_IRQHandler, Reserved111_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(DMA1_CH0_IRQHandler, DMA1_CH0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(DMA1_CH1_IRQHandler, DMA1_CH1_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(DMA1_CH2_IRQHandler, DMA1_CH2_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(DMA1_CH3_IRQHandler, DMA1_CH3_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved116_IRQHandler, Reserved116_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved117_IRQHandler, Reserved117_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved118_IRQHandler, Reserved118_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved119_IRQHandler, Reserved119_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved120_IRQHandler, Reserved120_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved121_IRQHandler, Reserved121_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved122_IRQHandler, Reserved122_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved123_IRQHandler, Reserved123_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved124_IRQHandler, Reserved124_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved125_IRQHandler, Reserved125_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(PKC_IRQHandler, PKC_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(SGI_IRQHandler, SGI_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(TRNG_IRQHandler, TRNG_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(ERR_SGI_UDF_TRNG_PKC_IRQHandler, ERR_SGI_UDF_TRNG_PKC_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(HYPERVISOR_IRQHandler, HYPERVISOR_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved131_IRQHandler, Reserved131_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved132_IRQHandler, Reserved132_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved133_IRQHandler, Reserved133_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved134_IRQHandler, Reserved134_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved135_IRQHandler, Reserved135_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved136_IRQHandler, Reserved136_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved137_IRQHandler, Reserved137_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved138_IRQHandler, Reserved138_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved139_IRQHandler, Reserved139_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved140_IRQHandler, Reserved140_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved141_IRQHandler, Reserved141_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved142_IRQHandler, Reserved142_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved143_IRQHandler, Reserved143_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(LPI2C0_AON_IRQHandler, LPI2C0_AON_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved145_IRQHandler, Reserved145_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(LPUART0_AON_IRQHandler, LPUART0_AON_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved147_IRQHandler, Reserved147_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(GPIO00_AON_IRQHandler, GPIO00_AON_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(GPIO01_AON_IRQHandler, GPIO01_AON_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved150_IRQHandler, Reserved150_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved151_IRQHandler, Reserved151_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved152_IRQHandler, Reserved152_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved153_IRQHandler, Reserved153_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved154_IRQHandler, Reserved154_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(LPCMP_IRQHandler, LPCMP_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(RTC_ALARM0_IRQHandler, RTC_ALARM0_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(RTC_ALARM1_IRQHandler, RTC_ALARM1_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(RTC_ALARM2_IRQHandler, RTC_ALARM2_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(RTC_WDT_IRQHandler, RTC_WDT_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(RTC_XTAL_IRQHandler, RTC_XTAL_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved161_IRQHandler, Reserved161_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved162_IRQHandler, Reserved162_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved163_IRQHandler, Reserved163_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(KPP_IRQHandler, KPP_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(LPADC_AON_IRQHandler, LPADC_AON_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved166_IRQHandler, Reserved166_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(TMR0_AON_IRQHandler, TMR0_AON_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(TMR1_AON_IRQHandler, TMR1_AON_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved169_IRQHandler, Reserved169_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(LCSENSE_IRQHandler, LCSENSE_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(LPTMR_AON_IRQHandler, LPTMR_AON_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved172_IRQHandler, Reserved172_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(CMP0_AON_IRQHandler, CMP0_AON_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(ADVC_IRQHandler, ADVC_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(Reserved175_IRQHandler, Reserved175_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(SGLCD_FRAME_AON_IRQHandler, SGLCD_FRAME_AON_DriverIRQHandler) 
DEFAULT_DRIVER_IRQ_HANDLER(SGLCD_FFAULT_AON_IRQHandler, SGLCD_FFAULT_AON_DriverIRQHandler) 

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
  Reset_Handler,                                   /* Reset Handler */
  NMI_Handler,                                     /* NMI Handler*/
  HardFault_Handler,                               /* Hard Fault Handler*/
  MemManage_Handler,                               /* MPU Fault Handler*/
  BusFault_Handler,                                /* Bus Fault Handler*/
  UsageFault_Handler,                              /* Usage Fault Handler*/
  SecureFault_Handler,                             /* Secure Fault Handler*/
  0,                                               /* Reserved*/
  0,                                               /* Reserved*/
  0,                                               /* Reserved*/
  SVC_Handler,                                     /* SVCall Handler*/
  DebugMon_Handler,                                /* Debug Monitor Handler*/
  0,                                               /* Reserved*/
  PendSV_Handler,                                  /* PendSV Handler*/
  SysTick_Handler,                                 /* SysTick Handler*/

/* External Interrupts*/
  Reserved16_IRQHandler,                           /* OR IRQ1 to IRQ79*/
  CMC_IRQHandler,                                  /* Core Mode Controller interrupt*/
  DMA0_CH0_IRQHandler,                             /* DMA3_0_CH0 error or transfer complete*/
  DMA0_CH1_IRQHandler,                             /* DMA3_0_CH1 error or transfer complete*/
  DMA0_CH2_IRQHandler,                             /* DMA3_0_CH2 error or transfer complete*/
  DMA0_CH3_IRQHandler,                             /* DMA3_0_CH3 error or transfer complete*/
  Reserved22_IRQHandler,                           /* xxx Interrupt 22*/
  Reserved23_IRQHandler,                           /* xxx Interrupt 23*/
  Reserved24_IRQHandler,                           /* xxx Interrupt 24*/
  Reserved25_IRQHandler,                           /* xxx Interrupt 25*/
  ERM0_IRQHandler,                                 /* ERM Single Bit error interrupt*/
  Reserved27_IRQHandler,                           /* xxx Interrupt 27*/
  FMU0_IRQHandler,                                 /* Flash Management Unit interrupt*/
  GLIKEY0_IRQHandler,                              /* GLIKEY Interrupt */
  MBC0_IRQHandler,                                 /* MBC secure violation interrupt*/
  SCG0_IRQHandler,                                 /* System Clock Generator interrupt*/
  Reserved32_IRQHandler,                           /* xxx Interrupt 32*/
  Reserved33_IRQHandler,                           /* xxx Interrupt 33*/
  WUU0_IRQHandler,                                 /* Wake Up Unit interrupt*/
  Reserved35_IRQHandler,                           /* xxx Interrupt 35*/
  Reserved36_IRQHandler,                           /* xxx Interrupt 36*/
  Reserved37_IRQHandler,                           /* xxx Interrupt 37*/
  Reserved38_IRQHandler,                           /* xxx Interrupt 38*/
  Reserved39_IRQHandler,                           /* xxx Interrupt 39*/
  Reserved40_IRQHandler,                           /* xxx Interrupt 40*/
  Reserved41_IRQHandler,                           /* xxx Interrupt 41*/
  LPI2C0_IRQHandler,                               /* Low-Power Inter Integrated Circuit interrupt*/
  LPI2C1_IRQHandler,                               /* Low-Power Inter Integrated Circuit interrupt*/
  LPSPI0_IRQHandler,                               /* Low-Power Serial Peripheral Interface interrupt*/
  LPSPI1_IRQHandler,                               /* Low-Power Serial Peripheral Interface interrupt*/
  Reserved46_IRQHandler,                           /* xxx Interrupt 46*/
  LPUART0_IRQHandler,                              /* Low-Power Universal Asynchronous Receive/Transmit interrupt*/
  LPUART1_IRQHandler,                              /* Low-Power Universal Asynchronous Receive/Transmit interrupt*/
  Reserved49_IRQHandler,                           /* xxx Interrupt 49*/
  Reserved50_IRQHandler,                           /* xxx Interrupt 50*/
  Reserved51_IRQHandler,                           /* xxx Interrupt 51*/
  Reserved52_IRQHandler,                           /* xxx Interrupt 52*/
  Reserved53_IRQHandler,                           /* xxx Interrupt 53*/
  CDOG0_IRQHandler,                                /* Code Watchdog Timer 0 interrupt*/
  CTIMER0_IRQHandler,                              /* Standard counter/timer 0 interrupt*/
  CTIMER1_IRQHandler,                              /* Standard counter/timer 1 interrupt*/
  CTIMER2_IRQHandler,                              /* Standard counter/timer 2 interrupt*/
  Reserved58_IRQHandler,                           /* xxx Interrupt 58*/
  Reserved59_IRQHandler,                           /* xxx Interrupt 59*/
  Reserved60_IRQHandler,                           /* xxx Interrupt 60*/
  Reserved61_IRQHandler,                           /* xxx Interrupt 61*/
  Reserved62_IRQHandler,                           /* xxx Interrupt 62*/
  Reserved63_IRQHandler,                           /* xxx Interrupt 63*/
  Reserved64_IRQHandler,                           /* xxx Interrupt 64*/
  Reserved65_IRQHandler,                           /* xxx Interrupt 65*/
  Reserved66_IRQHandler,                           /* xxx Interrupt 66*/
  Reserved67_IRQHandler,                           /* xxx Interrupt 67*/
  Reserved68_IRQHandler,                           /* xxx Interrupt 68*/
  Reserved69_IRQHandler,                           /* xxx Interrupt 69*/
  FREQME0_IRQHandler,                              /* Frequency Measurement interrupt*/
  Reserved71_IRQHandler,                           /* xxx Interrupt 71*/
  Reserved72_IRQHandler,                           /* xxx Interrupt 72*/
  OS_EVENT_IRQHandler,                             /* OS event timer interrupt*/
  Reserved74_IRQHandler,                           /* xxx Interrupt 74*/
  UTICK0_IRQHandler,                               /* Micro-Tick Timer interrupt*/
  WWDT0_IRQHandler,                                /* Windowed Watchdog Timer 0 interrupt*/
  Reserved77_IRQHandler,                           /* xxx Interrupt 77*/
  ADC0_IRQHandler,                                 /* Analog-to-Digital Converter interrupt*/
  Reserved79_IRQHandler,                           /* xxx Interrupt 79*/
  CMP0_IRQHandler,                                 /* Comparator interrupt*/
  Reserved81_IRQHandler,                           /* xxx Interrupt 81*/
  Reserved82_IRQHandler,                           /* xxx Interrupt 82*/
  Reserved83_IRQHandler,                           /* xxx Interrupt 83*/
  Reserved84_IRQHandler,                           /* xxx Interrupt 84*/
  Reserved85_IRQHandler,                           /* xxx Interrupt 85*/
  Reserved86_IRQHandler,                           /* xxx Interrupt 86*/
  GPIO10_IRQHandler,                               /* General Purpose Input/Output 1 interrupt 0*/
  GPIO11_IRQHandler,                               /* General Purpose Input/Output 1 interrupt 1*/
  GPIO20_IRQHandler,                               /* General Purpose Input/Output 2 interrupt 0*/
  GPIO21_IRQHandler,                               /* General Purpose Input/Output 2 interrupt 1*/
  GPIO30_IRQHandler,                               /* General Purpose Input/Output 3 interrupt 0*/
  GPIO31_IRQHandler,                               /* General Purpose Input/Output 3 interrupt 1*/
  Reserved93_IRQHandler,                           /* xxx Interrupt 93*/
  Reserved94_IRQHandler,                           /* xxx Interrupt 94*/
  Reserved95_IRQHandler,                           /* xxx Interrupt 95*/
  MU_A_TX_IRQHandler,                              /* Ored tx interrupt to MUA*/
  MU_A_RX_IRQHandler,                              /* Ored rx interrupt to MUA*/
  MU_A_INT_IRQHandler,                             /* ORed general purpose interrupt request to MUA*/
  CDOG1_IRQHandler,                                /* Code Watchdog Timer 1 interrupt*/
  Reserved100_IRQHandler,                          /* xxx Interrupt 100*/
  Reserved101_IRQHandler,                          /* xxx Interrupt 101*/
  Reserved102_IRQHandler,                          /* xxx Interrupt 102*/
  Reserved103_IRQHandler,                          /* xxx Interrupt 103*/
  Reserved104_IRQHandler,                          /* xxx Interrupt 104*/
  Reserved105_IRQHandler,                          /* xxx Interrupt 105*/
  Reserved106_IRQHandler,                          /* xxx Interrupt 106*/
  Reserved107_IRQHandler,                          /* xxx Interrupt 107*/
  Reserved108_IRQHandler,                          /* xxx Interrupt 108*/
  Reserved109_IRQHandler,                          /* xxx Interrupt 109*/
  Reserved110_IRQHandler,                          /* xxx Interrupt 110*/
  Reserved111_IRQHandler,                          /* xxx Interrupt 111*/
  DMA1_CH0_IRQHandler,                             /* DMA3_1_CH0 error or transfer complete*/
  DMA1_CH1_IRQHandler,                             /* DMA3_1_CH1 error or transfer complete*/
  DMA1_CH2_IRQHandler,                             /* DMA3_1_CH2 error or transfer complete*/
  DMA1_CH3_IRQHandler,                             /* DMA3_1_CH3 error or transfer complete*/
  Reserved116_IRQHandler,                          /* xxx Interrupt 116*/
  Reserved117_IRQHandler,                          /* xxx Interrupt 117*/
  Reserved118_IRQHandler,                          /* xxx Interrupt 118*/
  Reserved119_IRQHandler,                          /* xxx Interrupt 119*/
  Reserved120_IRQHandler,                          /* xxx Interrupt 120*/
  Reserved121_IRQHandler,                          /* xxx Interrupt 121*/
  Reserved122_IRQHandler,                          /* xxx Interrupt 122*/
  Reserved123_IRQHandler,                          /* xxx Interrupt 123*/
  Reserved124_IRQHandler,                          /* xxx Interrupt 124*/
  Reserved125_IRQHandler,                          /* xxx Interrupt 125*/
  PKC_IRQHandler,                                  /* PKC interrupt*/
  SGI_IRQHandler,                                  /* SGI interrupt*/
  TRNG_IRQHandler,                                 /* TRNG interrupt*/
  ERR_SGI_UDF_TRNG_PKC_IRQHandler,                 /* ORed Error Interrupt from SGI, UDF,TRNG and PKC*/
  HYPERVISOR_IRQHandler,                           /* Hypervisor interrupt*/
  Reserved131_IRQHandler,                          /* xxx Interrupt 131*/
  Reserved132_IRQHandler,                          /* xxx Interrupt 132*/
  Reserved133_IRQHandler,                          /* xxx Interrupt 133*/
  Reserved134_IRQHandler,                          /* xxx Interrupt 134*/
  Reserved135_IRQHandler,                          /* xxx Interrupt 135*/
  Reserved136_IRQHandler,                          /* xxx Interrupt 136*/
  Reserved137_IRQHandler,                          /* xxx Interrupt 137*/
  Reserved138_IRQHandler,                          /* xxx Interrupt 138*/
  Reserved139_IRQHandler,                          /* xxx Interrupt 139*/
  Reserved140_IRQHandler,                          /* xxx Interrupt 140*/
  Reserved141_IRQHandler,                          /* xxx Interrupt 141*/
  Reserved142_IRQHandler,                          /* xxx Interrupt 142*/
  Reserved143_IRQHandler,                          /* xxx Interrupt 143*/
  LPI2C0_AON_IRQHandler,                           /* Low-Power Inter Integrated Circuit interrupt*/
  Reserved145_IRQHandler,                          /* xxx Interrupt 145*/
  LPUART0_AON_IRQHandler,                          /* Low-Power Universal Asynchronous Receive/Transmit interrupt*/
  Reserved147_IRQHandler,                          /* xxx Interrupt 147*/
  GPIO00_AON_IRQHandler,                           /* General Purpose Input/Output interrupt 0*/
  GPIO01_AON_IRQHandler,                           /* General Purpose Input/Output interrupt 1*/
  Reserved150_IRQHandler,                          /* xxx Interrupt 150*/
  Reserved151_IRQHandler,                          /* xxx Interrupt 151*/
  Reserved152_IRQHandler,                          /* xxx Interrupt 152*/
  Reserved153_IRQHandler,                          /* xxx Interrupt 153*/
  Reserved154_IRQHandler,                          /* xxx Interrupt 154*/
  LPCMP_IRQHandler,                                /* Comparator*/
  RTC_ALARM0_IRQHandler,                           /* RTC alarm 0*/
  RTC_ALARM1_IRQHandler,                           /* RTC alarm 1*/
  RTC_ALARM2_IRQHandler,                           /* RTC alarm 2*/
  RTC_WDT_IRQHandler,                              /* RTC watchdog*/
  RTC_XTAL_IRQHandler,                             /* RTC XTAL Fail*/
  Reserved161_IRQHandler,                          /* xxx Interrupt 161*/
  Reserved162_IRQHandler,                          /* xxx Interrupt 162*/
  Reserved163_IRQHandler,                          /* xxx Interrupt 163*/
  KPP_IRQHandler,                                  /* Keypad Interrupt*/
  LPADC_AON_IRQHandler,                            /* ADC Summary Interrupt*/
  Reserved166_IRQHandler,                          /* xxx Interrupt 166*/
  TMR0_AON_IRQHandler,                             /* ORed QTMR Interrupts*/
  TMR1_AON_IRQHandler,                             /* ORed QTMR Interrupts*/
  Reserved169_IRQHandler,                          /* xxx Interrupt 169*/
  LCSENSE_IRQHandler,                              /* LCSense Fault/Tamper Interrupt*/
  LPTMR_AON_IRQHandler,                            /* Low Power Timer 0 interrupt*/
  Reserved172_IRQHandler,                          /* xxx Interrupt 172*/
  CMP0_AON_IRQHandler,                             /* Comparator interrupt*/
  ADVC_IRQHandler,                                 /* ADVC_2.0 Controller Interrupt*/
  Reserved175_IRQHandler,                          /* xxx Interrupt 175*/
  SGLCD_FRAME_AON_IRQHandler,                      /* Frame Update Interrupt*/
  SGLCD_FFAULT_AON_IRQHandler,                     /* Fault Detect Interrupt*/
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
