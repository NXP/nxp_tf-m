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
 DEFAULT_DRIVER_IRQ_HANDLER(Reserved16_IRQHandler,  Reserved16_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(Reserved17_IRQHandler,  Reserved17_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(Reserved18_IRQHandler, Reserved18_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(FRO2_IRQHandler,       FRO2_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(GLIKEY4_IRQHandler,    GLIKEY4_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(UTICK1_IRQHandler,     UTICK1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(MRT1_IRQHandler,       MRT1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(CTIMER5_IRQHandler,    CTIMER5_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(CTIMER6_IRQHandler,    CTIMER6_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(CTIMER7_IRQHandler,    CTIMER7_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(LPI2C15_IRQHandler,    LPI2C15_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM17_IRQHandler, LP_FLEXCOMM17_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM18_IRQHandler, LP_FLEXCOMM18_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM19_IRQHandler, LP_FLEXCOMM19_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM20_IRQHandler, LP_FLEXCOMM20_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(ADC_IRQHandler,        ADC_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(SDADC_IRQHandler,      SDADC_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(GLIKEY1_IRQHandler,    GLIKEY1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(ACMP_IRQHandler,       ACMP_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(PDM_EVENT_IRQHandler,  PDM_EVENT_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(HYPERVISOR_IRQHandler, HYPERVISOR_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(SECURE_VIOLATION_IRQHandler, SECURE_VIOLATION_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(PDM_HWVAD_EVENT_IRQHandler, PDM_HWVAD_EVENT_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(RTC1_ALARM_IRQHandler, RTC1_ALARM_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(RTC1_IRQHandler,       RTC1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(HIFI1_IRQHandler,      HIFI1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(MU1_B_IRQHandler,      MU1_B_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(MU2_B_IRQHandler,      MU2_B_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(MU3_A_IRQHandler,      MU3_A_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(PMC_IRQHandler,        PMC_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(OS_EVENT_IRQHandler,   OS_EVENT_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(USDHC0_IRQHandler,     USDHC0_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(USDHC1_IRQHandler,     USDHC1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(I3C2_IRQHandler,       I3C2_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(USB0_IRQHandler,       USB0_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(USB1_IRQHandler,       USB1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(WDT2_IRQHandler,       WDT2_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(WDT3_IRQHandler,       WDT3_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(USBPHY0_IRQHandler,    USBPHY0_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(Reserved55_IRQHandler, Reserved55_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(I3C3_IRQHandler,       I3C3_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(FLEXIO_IRQHandler,     FLEXIO_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(LCDIF_IRQHandler,      LCDIF_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(VGPU_IRQHandler,       VGPU_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(MIPI_IRQHandler,       MIPI_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA2_CH0_IRQHandler,  EDMA2_CH0_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA2_CH1_IRQHandler,  EDMA2_CH1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA2_CH2_IRQHandler,  EDMA2_CH2_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA2_CH3_IRQHandler,  EDMA2_CH3_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA2_CH4_IRQHandler,  EDMA2_CH4_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA2_CH5_IRQHandler,  EDMA2_CH5_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA2_CH6_IRQHandler,  EDMA2_CH6_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA2_CH7_IRQHandler,  EDMA2_CH7_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA3_CH0_IRQHandler,  EDMA3_CH0_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA3_CH1_IRQHandler,  EDMA3_CH1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA3_CH2_IRQHandler,  EDMA3_CH2_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA3_CH3_IRQHandler,  EDMA3_CH3_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA3_CH4_IRQHandler,  EDMA3_CH4_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA3_CH5_IRQHandler,  EDMA3_CH5_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA3_CH6_IRQHandler,  EDMA3_CH6_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EDMA3_CH7_IRQHandler,  EDMA3_CH7_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(GPIO80_IRQHandler,     GPIO80_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(GPIO81_IRQHandler,     GPIO81_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(GPIO90_IRQHandler,     GPIO90_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(GPIO91_IRQHandler,     GPIO91_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(GPIO100_IRQHandler,    GPIO100_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(GPIO101_IRQHandler,    GPIO101_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(Reserved83_IRQHandler, Reserved83_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(Reserved84_IRQHandler, Reserved84_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT0_IRQHandler,   PIN_INT0_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT1_IRQHandler,   PIN_INT1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT2_IRQHandler,   PIN_INT2_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT3_IRQHandler,   PIN_INT3_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(SAI3_IRQHandler,       SAI3_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(XSPI2_IRQHandler,      XSPI2_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(MMU2_IRQHandler,       MMU2_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(Reserved92_IRQHandler, Reserved92_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(GDET3_IRQHandler,      GDET3_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(CDOG3_IRQHandler,      CDOG3_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(CDOG4_IRQHandler,      CDOG4_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(Reserved96_IRQHandler, Reserved96_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(CPU1_IRQHandler,       CPU1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(JPEGDEC_IRQHandler,    JPEGDEC_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(PNGDEC_IRQHandler,     PNGDEC_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(Reserved100_IRQHandler,Reserved100_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(TEMPDET0_IRQHandler,   TEMPDET0_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(TEMPDET1_IRQHandler,   TEMPDET1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(EZHV_IRQHandler,       EZHV_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(SLEEPCON1_IRQHandler,  SLEEPCON1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(PVTS1_CPU1_IRQHandler, PVTS1_CPU1_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(Reserved106_IRQHandler,Reserved106_DriverIRQHandler)
 DEFAULT_DRIVER_IRQ_HANDLER(PVTS1_HIFI1_IRQHandler,PVTS1_HIFI1_DriverIRQHandler)

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
    Reset_Handler,                      // The reset handler

    NMI_Handler,                        // NMI Handler
    HardFault_Handler,                  // Hard Fault Handler
    MemManage_Handler,                  // MPU Fault Handler
    BusFault_Handler,                   // Bus Fault Handler
    UsageFault_Handler,                 // Usage Fault Handler
    SecureFault_Handler,                // Secure Fault Handler
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    SVC_Handler,                        // SVCall Handler
    DebugMon_Handler,                   // Debug Monitor Handler
    0,                                  // Reserved
    PendSV_Handler,                     // PendSV Handler
    SysTick_Handler,                    // SysTick Handler

    // Chip Level - MIMXRT798S_cm33_core1
    Reserved16_IRQHandler,       // 16 : Reserved interrupt
    Reserved17_IRQHandler,       // 17 : Reserved interrupt
    Reserved18_IRQHandler,       // 18 : Reserved interrupt
    FRO2_IRQHandler,             // 19 : FRO: 300MHz FRO in VDD1 domain Async interrupt
    GLIKEY4_IRQHandler,          // 20 : GLIKEY: Interrupt
    UTICK1_IRQHandler,           // 21 : Micro-tick Timer
    MRT1_IRQHandler,             // 22 : MRT: Ored Interrupt request
    CTIMER5_IRQHandler,          // 23 : CTIMER: Interrupt request
    CTIMER6_IRQHandler,          // 24 : CTIMER: Interrupt request
    CTIMER7_IRQHandler,          // 25 : CTIMER: Interrupt request
    LPI2C15_IRQHandler,          // 26 : LPI2C: Interrupt request
    LP_FLEXCOMM17_IRQHandler,    // 27 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM18_IRQHandler,    // 28 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM19_IRQHandler,    // 29 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM20_IRQHandler,    // 30 : FLEXCOMM: Interrupt request
    ADC_IRQHandler,              // 31 : ADC0: Interrupt request
    SDADC_IRQHandler,            // 32 : SDADC0: Interrupt request
    GLIKEY1_IRQHandler,          // 33 : GLIKEY: Interrupt
    ACMP_IRQHandler,             // 34 : ACMP: interrupt request
    PDM_EVENT_IRQHandler,        // 35 : MIC: Interrupt request for read data or Error
    HYPERVISOR_IRQHandler,       // 36 : SECURE: Sense domain Hypervisor interrupt
    SECURE_VIOLATION_IRQHandler, // 37 : SECURE: Sense domain Secure violation interrupt
    PDM_HWVAD_EVENT_IRQHandler,  // 38 : MIC: Hardware Voice Activity Detector interrupt or error interrupt
    RTC1_ALARM_IRQHandler,       // 39 : RTC: Alarm interrupt
    RTC1_IRQHandler,             // 40 : RTC: wakeup interrupt to Sense domain
    HIFI1_IRQHandler,            // 41 : HIFI1: Interrupt request
    MU1_B_IRQHandler,            // 42 : MU1: MUB, CPU1 to CPU0
    MU2_B_IRQHandler,            // 43 : MU2: MUB, CPU1  to HiFi4
    MU3_A_IRQHandler,            // 44 : MU3: MUA, CPU1 to HiFi1
    PMC_IRQHandler,              // 45 : PMC: Sense domain Interrupt
    OS_EVENT_IRQHandler,         // 46 : OSEVENT: Event timer CPU1 Wakeup/interrupt
    USDHC0_IRQHandler,           // 47 : USDHC: Interrupt request
    USDHC1_IRQHandler,           // 48 : USDHC: Interrupt request
    I3C2_IRQHandler,             // 49 : I3C: Interrupt Request
    USB0_IRQHandler,             // 50 : USB: HSUSB Interrup request
    USB1_IRQHandler,             // 51 : USB: eUSB Interrup request
    WDT2_IRQHandler,             // 52 : WDT: Interrupt request
    WDT3_IRQHandler,             // 53 : WDT: Interrupt request
    USBPHY0_IRQHandler,          // 54 : HSUSBPHY: UTM interrupt request
    Reserved55_IRQHandler,       // 55 : Reserved interrupt
    I3C3_IRQHandler,             // 56 : I3C: Interrupt Request
    FLEXIO_IRQHandler,           // 57 : flexio: Interrupt request
    LCDIF_IRQHandler,            // 58 : dcn: Interrupt request
    VGPU_IRQHandler,             // 59 : VGPU interrupt from graphics core
    MIPI_IRQHandler,             // 60 : DSI: Interrupt request
    EDMA2_CH0_IRQHandler,        // 61 : edma2: Channel 0 interrupt
    EDMA2_CH1_IRQHandler,        // 62 : edma2: Channel 1 interrupt
    EDMA2_CH2_IRQHandler,        // 63 : edma2: Channel 2 interrupt
    EDMA2_CH3_IRQHandler,        // 64 : edma2: Channel 3 interrupt
    EDMA2_CH4_IRQHandler,        // 65 : edma2: Channel 4 interrupt
    EDMA2_CH5_IRQHandler,        // 66 : edma2: Channel 5 interrupt
    EDMA2_CH6_IRQHandler,        // 67 : edma2: Channel 6 interrupt
    EDMA2_CH7_IRQHandler,        // 68 : edma2: Channel 7 interrupt
    EDMA3_CH0_IRQHandler,        // 69 : edma3: Channel 0 interrupt
    EDMA3_CH1_IRQHandler,        // 70 : edma3: Channel 1 interrupt
    EDMA3_CH2_IRQHandler,        // 71 : edma3: Channel 2 interrupt
    EDMA3_CH3_IRQHandler,        // 72 : edma3: Channel 3 interrupt
    EDMA3_CH4_IRQHandler,        // 73 : edma3: Channel 4 interrupt
    EDMA3_CH5_IRQHandler,        // 74 : edma3: Channel 5 interrupt
    EDMA3_CH6_IRQHandler,        // 75 : edma3: Channel 6 interrupt
    EDMA3_CH7_IRQHandler,        // 76 : edma3: Channel 7 interrupt
    GPIO80_IRQHandler,           // 77 : rgpio8: Interupt request, channel 0
    GPIO81_IRQHandler,           // 78 : rgpio8: Interupt request, channel 1
    GPIO90_IRQHandler,           // 79 : rgpio9: Interupt request, channel 0
    GPIO91_IRQHandler,           // 80 : rgpio9: Interupt request, channel 1
    GPIO100_IRQHandler,          // 81 : rgpio10: Interupt request, channel 0
    GPIO101_IRQHandler,          // 82 : rgpio10: Interupt request, channel 1
    Reserved83_IRQHandler,       // 83 : Reserved interrupt
    Reserved84_IRQHandler,       // 84 : Reserved interrupt
    PIN_INT0_IRQHandler,         // 85 : pint1: Interupt request 0
    PIN_INT1_IRQHandler,         // 86 : pint1: Interupt request 1
    PIN_INT2_IRQHandler,         // 87 : pint1: Interupt request 2
    PIN_INT3_IRQHandler,         // 88 : pint1: Interupt request 3
    SAI3_IRQHandler,             // 89 : sai3: TX/RX interrupt
    XSPI2_IRQHandler,            // 90 : xspi2: Ored interrupt
    MMU2_IRQHandler,             // 91 : mmu2: Interrupt request
    Reserved92_IRQHandler,       // 92 : Reserved interrupt
    GDET3_IRQHandler,            // 93 : GDET3: Interrupt request
    CDOG3_IRQHandler,            // 94 : cdog3: Interrupt request
    CDOG4_IRQHandler,            // 95 : cdog4: Interrupt request
    Reserved96_IRQHandler,       // 96 : Reserved interrupt
    CPU1_IRQHandler,             // 97 : CTI interrupt request
    JPEGDEC_IRQHandler,          // 98 : JPEGDEC Ored context 0-3 interrupt request
    PNGDEC_IRQHandler,           // 99 : PNGDEC interrupt request
    Reserved100_IRQHandler,      // 100: Reserved interrupt
    TEMPDET0_IRQHandler,         // 101: TEMPDET0: Temperature Detect Interrupt request 0
    TEMPDET1_IRQHandler,         // 102: TEMPDET1: Temperature Detect Interrupt request 1
    EZHV_IRQHandler,             // 103: ezhv: Interrupt request
    SLEEPCON1_IRQHandler,        // 104: SLEEPCON_SENSE CPU1 wakeup event
    PVTS1_CPU1_IRQHandler,       // 105: PVTS1 CPU1 interrupt
    Reserved106_IRQHandler,      // 106: Reserved interrupt
    PVTS1_HIFI1_IRQHandler,      // 107: PVTS1 HIFI1 interrupt
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

     
    /* Set Vector Table Offset Register (VTOR)*/
    SCB->VTOR = (uint32_t) &(__VECTOR_TABLE[0]);

     SystemInit();                             /* CMSIS System Initialization */
     __PROGRAM_START();                        /* Enter PreMain (C library entry point) */

 }

//////////  default startup file from c code /////////////

