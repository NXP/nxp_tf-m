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
DEFAULT_DRIVER_IRQ_HANDLER(Reserved16_IRQHandler,             Reserved16_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(UTICK0_IRQHandler,                 UTICK0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MRT0_IRQHandler,                   MRT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER0_IRQHandler,                CTIMER0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER1_IRQHandler,                CTIMER1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SCT0_IRQHandler,                   SCT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER3_IRQHandler,                CTIMER3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM0_IRQHandler,           LP_FLEXCOMM0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM1_IRQHandler,           LP_FLEXCOMM1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM2_IRQHandler,           LP_FLEXCOMM2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM3_IRQHandler,           LP_FLEXCOMM3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM4_IRQHandler,           LP_FLEXCOMM4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM5_IRQHandler,           LP_FLEXCOMM5_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPSPI14_IRQHandler,                LPSPI14_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPI2C15_IRQHandler,                LPI2C15_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ADC_IRQHandler,                    ADC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SDADC_IRQHandler,                  SDADC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ACMP_IRQHandler,                   ACMP_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PDM_EVENT_IRQHandler,              PDM_EVENT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PDM_HWVAD_EVENT_IRQHandler,        PDM_HWVAD_EVENT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(HYPERVISOR_IRQHandler,             HYPERVISOR_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SECURE_VIOLATION_IRQHandler,       SECURE_VIOLATION_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SENSE_HYPERVISOR_IRQHandler,       SENSE_HYPERVISOR_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SENSE_SECURE_VIOLATION_IRQHandler, SENSE_SECURE_VIOLATION_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MEDIA_HYPERVISOR_IRQHandler,       MEDIA_HYPERVISOR_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MEDIA_SECURE_VIOLATION_IRQHandler, MEDIA_SECURE_VIOLATION_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RTC0_ALARM_IRQHandler,             RTC0_ALARM_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(RTC0_IRQHandler,                   RTC0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(HIFI4_IRQHandler,                  HIFI4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MU0_A_IRQHandler,                  MU0_A_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MU1_A_IRQHandler,                  MU1_A_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MU4_A_IRQHandler,                  MU4_A_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER2_IRQHandler,                CTIMER2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CTIMER4_IRQHandler,                CTIMER4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(OS_EVENT_IRQHandler,               OS_EVENT_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM6_IRQHandler,           LP_FLEXCOMM6_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM7_IRQHandler,           LP_FLEXCOMM7_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(USDHC0_IRQHandler,                 USDHC0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(USDHC1_IRQHandler,                 USDHC1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(I3C0_IRQHandler,                   I3C0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(USB0_IRQHandler,                   USB0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(USB1_IRQHandler,                   USB1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WDT0_IRQHandler,                   WDT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(WDT1_IRQHandler,                   WDT1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(USBPHY0_IRQHandler,                USBPHY0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PUF_IRQHandler,                    PUF_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved62_IRQHandler,             Reserved62_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM8_IRQHandler,           LP_FLEXCOMM8_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM9_IRQHandler,           LP_FLEXCOMM9_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM10_IRQHandler,          LP_FLEXCOMM10_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM11_IRQHandler,          LP_FLEXCOMM11_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM12_IRQHandler,          LP_FLEXCOMM12_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LP_FLEXCOMM13_IRQHandler,          LP_FLEXCOMM13_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LPSPI16_IRQHandler,                LPSPI16_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(I3C1_IRQHandler,                   I3C1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FLEXIO_IRQHandler,                 FLEXIO_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(LCDIF_IRQHandler,                  LCDIF_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(VGPU_IRQHandler,                   VGPU_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MIPI_IRQHandler,                   MIPI_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH0_IRQHandler,              EDMA0_CH0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH1_IRQHandler,              EDMA0_CH1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH2_IRQHandler,              EDMA0_CH2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH3_IRQHandler,              EDMA0_CH3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH4_IRQHandler,              EDMA0_CH4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH5_IRQHandler,              EDMA0_CH5_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH6_IRQHandler,              EDMA0_CH6_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH7_IRQHandler,              EDMA0_CH7_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH8_IRQHandler,              EDMA0_CH8_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH9_IRQHandler,              EDMA0_CH9_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH10_IRQHandler,             EDMA0_CH10_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH11_IRQHandler,             EDMA0_CH11_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH12_IRQHandler,             EDMA0_CH12_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH13_IRQHandler,             EDMA0_CH13_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH14_IRQHandler,             EDMA0_CH14_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA0_CH15_IRQHandler,             EDMA0_CH15_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH0_IRQHandler,              EDMA1_CH0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH1_IRQHandler,              EDMA1_CH1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH2_IRQHandler,              EDMA1_CH2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH3_IRQHandler,              EDMA1_CH3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH4_IRQHandler,              EDMA1_CH4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH5_IRQHandler,              EDMA1_CH5_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH6_IRQHandler,              EDMA1_CH6_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH7_IRQHandler,              EDMA1_CH7_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH8_IRQHandler,              EDMA1_CH8_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH9_IRQHandler,              EDMA1_CH9_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH10_IRQHandler,             EDMA1_CH10_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH11_IRQHandler,             EDMA1_CH11_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH12_IRQHandler,             EDMA1_CH12_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH13_IRQHandler,             EDMA1_CH13_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH14_IRQHandler,             EDMA1_CH14_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EDMA1_CH15_IRQHandler,             EDMA1_CH15_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO00_IRQHandler,                 GPIO00_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO01_IRQHandler,                 GPIO01_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO10_IRQHandler,                 GPIO10_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO11_IRQHandler,                 GPIO11_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO20_IRQHandler,                 GPIO20_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO21_IRQHandler,                 GPIO21_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO30_IRQHandler,                 GPIO30_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO31_IRQHandler,                 GPIO31_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO40_IRQHandler,                 GPIO40_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO41_IRQHandler,                 GPIO41_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO50_IRQHandler,                 GPIO50_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO51_IRQHandler,                 GPIO51_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO60_IRQHandler,                 GPIO60_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO61_IRQHandler,                 GPIO61_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO70_IRQHandler,                 GPIO70_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GPIO71_IRQHandler,                 GPIO71_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT0_IRQHandler,               PIN_INT0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT1_IRQHandler,               PIN_INT1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT2_IRQHandler,               PIN_INT2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT3_IRQHandler,               PIN_INT3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT4_IRQHandler,               PIN_INT4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT5_IRQHandler,               PIN_INT5_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT6_IRQHandler,               PIN_INT6_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PIN_INT7_IRQHandler,               PIN_INT7_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SAI0_IRQHandler,                   SAI0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SAI1_IRQHandler,                   SAI1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SAI2_IRQHandler,                   SAI2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(XSPI0_IRQHandler,                  XSPI0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(XSPI1_IRQHandler,                  XSPI1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(XSPI2_IRQHandler,                  XSPI2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MMU0_IRQHandler,                   MMU0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MMU1_IRQHandler,                   MMU1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(MMU2_IRQHandler,                   MMU2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Freqme_IRQHandler,                 Freqme_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GDET0_IRQHandler,                  GDET0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved142_IRQHandler,            Reserved142_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved143_IRQHandler,            Reserved143_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GDET3_IRQHandler,                  GDET3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CDOG0_IRQHandler,                  CDOG0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CDOG1_IRQHandler,                  CDOG1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CDOG2_IRQHandler,                  CDOG2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ITRC0_IRQHandler,                  ITRC0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(ELS_IRQHandler,                    ELS_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PKC_IRQHandler,                    PKC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(OSCCA_IRQHandler,                  OSCCA_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(SLEEPCON0_IRQHandler,              SLEEPCON0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(CPU0_IRQHandler,                   CPU0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PMC_IRQHandler,                    PMC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(JPEGDEC_IRQHandler,                JPEGDEC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PNGDEC_IRQHandler,                 PNGDEC_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(NPU_IRQHandler,                    NPU_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(TRNG_IRQHandler,                   TRNG_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(TEMPDET0_IRQHandler,               TEMPDET0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(TEMPDET1_IRQHandler,               TEMPDET1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(EZHV_IRQHandler,                   EZHV_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GLIKEY0_IRQHandler,                GLIKEY0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GLIKEY2_IRQHandler,                GLIKEY2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GLIKEY3_IRQHandler,                GLIKEY3_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(GLIKEY5_IRQHandler,                GLIKEY5_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PVTS0_CPU0_IRQHandler,             PVTS0_CPU0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved167_IRQHandler,            Reserved167_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(PVTS0_HIFI4_IRQHandler,            PVTS0_HIFI4_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(Reserved169_IRQHandler,            Reserved169_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FRO0_IRQHandler,                   FRO0_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FRO1_IRQHandler,                   FRO1_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(FRO2_IRQHandler,                   FRO2_DriverIRQHandler)
DEFAULT_DRIVER_IRQ_HANDLER(OCOTP_IRQHandler,                  OCOTP_DriverIRQHandler)

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

    // Chip Level - MIMXRT798S_cm33_core0
    Reserved16_IRQHandler,             // 16 : Reserved interrupt
    UTICK0_IRQHandler,                 // 17 : Micro-tick Timer
    MRT0_IRQHandler,                   // 18 : MRT: Ored Interrupt request
    CTIMER0_IRQHandler,                // 19 : CTIMER: Interrupt request
    CTIMER1_IRQHandler,                // 20 : CTIMER: Interrupt request
    SCT0_IRQHandler,                   // 21 : SCT: Interrupt request
    CTIMER3_IRQHandler,                // 22 : CTIMER: Interrupt request
    LP_FLEXCOMM0_IRQHandler,           // 23 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM1_IRQHandler,           // 24 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM2_IRQHandler,           // 25 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM3_IRQHandler,           // 26 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM4_IRQHandler,           // 27 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM5_IRQHandler,           // 28 : FLEXCOMM: Interrupt request
    LPSPI14_IRQHandler,                // 29 : LPSPI: Interrupt request
    LPI2C15_IRQHandler,                // 30 : LPI2C: Interrupt request
    ADC_IRQHandler,                    // 31 : ADC0: Interrupt request
    SDADC_IRQHandler,                  // 32 : SDADC0: Interrupt request
    ACMP_IRQHandler,                   // 33 : ACMP: interrupt request
    PDM_EVENT_IRQHandler,              // 34 : MIC: Interrupt request for read data or Error
    PDM_HWVAD_EVENT_IRQHandler,        // 35 : MIC: Hardware Voice Activity Detector interrupt or error interrupt
    HYPERVISOR_IRQHandler,             // 36 : SECURE: Compute domain P-Bus Hypervisor interrupt
    SECURE_VIOLATION_IRQHandler,       // 37 : SECURE: Compute domain P-Bus Secure violation interrupt
    SENSE_HYPERVISOR_IRQHandler,       // 38 : Sense domain Hypervisor interrupt
    SENSE_SECURE_VIOLATION_IRQHandler, // 39 : Sense domain Secure violation interrupt
    MEDIA_HYPERVISOR_IRQHandler,       // 40 : Media domain Hypervisor interrupt
    MEDIA_SECURE_VIOLATION_IRQHandler, // 41 : Media domain Secure violation interrupt
    RTC0_ALARM_IRQHandler,             // 42 : RTC: Alarm interrupt
    RTC0_IRQHandler,                   // 43 : RTC: wakeup interrupt to Compute domain
    HIFI4_IRQHandler,                  // 44 : HIFI4: Interrupt request
    MU0_A_IRQHandler,                  // 45 : MU0: MUA, CPU0 to HiFi1
    MU1_A_IRQHandler,                  // 46 : MU1: MUA, CPU0 to CPU1
    MU4_A_IRQHandler,                  // 47 : MU4: MUA, CPU0 to HiFi4
    CTIMER2_IRQHandler,                // 48 : CTIMER: Interrupt request
    CTIMER4_IRQHandler,                // 49 : CTIMER: Interrupt request
    OS_EVENT_IRQHandler,               // 50 : OSEVENT: Event timer CPU0 Wakeup/interrupt
    LP_FLEXCOMM6_IRQHandler,           // 51 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM7_IRQHandler,           // 52 : FLEXCOMM: Interrupt request
    USDHC0_IRQHandler,                 // 53 : USDHC: Interrupt request
    USDHC1_IRQHandler,                 // 54 : USDHC: Interrupt request
    I3C0_IRQHandler,                   // 55 : I3C: Interrupt Request
    USB0_IRQHandler,                   // 56 : USB: HSUSB Interrup request
    USB1_IRQHandler,                   // 57 : USB: eUSB Interrup request
    WDT0_IRQHandler,                   // 58 : WDT: Interrupt request
    WDT1_IRQHandler,                   // 59 : WDT: Interrupt request
    USBPHY0_IRQHandler,                // 60 : HSUSBPHY: UTM interrupt request
    PUF_IRQHandler,                    // 61 : PUF: Interrupt Request
    Reserved62_IRQHandler,             // 62 : Reserved interrupt
    LP_FLEXCOMM8_IRQHandler,           // 63 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM9_IRQHandler,           // 64 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM10_IRQHandler,          // 65 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM11_IRQHandler,          // 66 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM12_IRQHandler,          // 67 : FLEXCOMM: Interrupt request
    LP_FLEXCOMM13_IRQHandler,          // 68 : FLEXCOMM: Interrupt request
    LPSPI16_IRQHandler,                // 69 : LPSPI: Interrupt request
    I3C1_IRQHandler,                   // 70 : I3C: Interrupt Request
    FLEXIO_IRQHandler,                 // 71 : FLEXIO: Interrupt request
    LCDIF_IRQHandler,                  // 72 : LCDIF: Interrupt request
    VGPU_IRQHandler,                   // 73 : VGPU: Interrupt from graphics core (Domain HCLK)
    MIPI_IRQHandler,                   // 74 : DSI: Interrupt request
    EDMA0_CH0_IRQHandler,              // 75 : EDMA: Channel 0 interrupt
    EDMA0_CH1_IRQHandler,              // 76 : EDMA: Channel 1 interrupt
    EDMA0_CH2_IRQHandler,              // 77 : EDMA: Channel 2 interrupt
    EDMA0_CH3_IRQHandler,              // 78 : EDMA: Channel 3 interrupt
    EDMA0_CH4_IRQHandler,              // 79 : EDMA: Channel 4 interrupt
    EDMA0_CH5_IRQHandler,              // 80 : EDMA: Channel 5 interrupt
    EDMA0_CH6_IRQHandler,              // 81 : EDMA: Channel 6 interrupt
    EDMA0_CH7_IRQHandler,              // 82 : EDMA: Channel 7 interrupt
    EDMA0_CH8_IRQHandler,              // 83 : EDMA: Channel 8 interrupt
    EDMA0_CH9_IRQHandler,              // 84 : EDMA: Channel 9 interrupt
    EDMA0_CH10_IRQHandler,             // 85 : EDMA: Channel 10 interrupt
    EDMA0_CH11_IRQHandler,             // 86 : EDMA: Channel 11 interrupt
    EDMA0_CH12_IRQHandler,             // 87 : EDMA: Channel 12 interrupt
    EDMA0_CH13_IRQHandler,             // 88 : EDMA: Channel 13 interrupt
    EDMA0_CH14_IRQHandler,             // 89 : EDMA: Channel 14 interrupt
    EDMA0_CH15_IRQHandler,             // 90 : EDMA: Channel 15 interrupt
    EDMA1_CH0_IRQHandler,              // 91 : EDMA: Channel 0 interrupt
    EDMA1_CH1_IRQHandler,              // 92 : EDMA: Channel 1 interrupt
    EDMA1_CH2_IRQHandler,              // 93 : EDMA: Channel 2 interrupt
    EDMA1_CH3_IRQHandler,              // 94 : EDMA: Channel 3 interrupt
    EDMA1_CH4_IRQHandler,              // 95 : EDMA: Channel 4 interrupt
    EDMA1_CH5_IRQHandler,              // 96 : EDMA: Channel 5 interrupt
    EDMA1_CH6_IRQHandler,              // 97 : EDMA: Channel 6 interrupt
    EDMA1_CH7_IRQHandler,              // 98 : EDMA: Channel 7 interrupt
    EDMA1_CH8_IRQHandler,              // 99 : EDMA: Channel 8 interrupt
    EDMA1_CH9_IRQHandler,              // 100: EDMA: Channel 9 interrupt
    EDMA1_CH10_IRQHandler,             // 101: EDMA: Channel 10 interrupt
    EDMA1_CH11_IRQHandler,             // 102: EDMA: Channel 11 interrupt
    EDMA1_CH12_IRQHandler,             // 103: EDMA: Channel 12 interrupt
    EDMA1_CH13_IRQHandler,             // 104: EDMA: Channel 13 interrupt
    EDMA1_CH14_IRQHandler,             // 105: EDMA: Channel 14 interrupt
    EDMA1_CH15_IRQHandler,             // 106: EDMA: Channel 15 interrupt
    GPIO00_IRQHandler,                 // 107: GPIO: Interupt request, channel 0
    GPIO01_IRQHandler,                 // 108: GPIO: Interupt request, channel 1
    GPIO10_IRQHandler,                 // 109: GPIO: Interupt request, channel 0
    GPIO11_IRQHandler,                 // 110: GPIO: Interupt request, channel 1
    GPIO20_IRQHandler,                 // 111: GPIO: Interupt request, channel 0
    GPIO21_IRQHandler,                 // 112: GPIO: Interupt request, channel 1
    GPIO30_IRQHandler,                 // 113: GPIO: Interupt request, channel 0
    GPIO31_IRQHandler,                 // 114: GPIO: Interupt request, channel 1
    GPIO40_IRQHandler,                 // 115: GPIO: Interupt request, channel 0
    GPIO41_IRQHandler,                 // 116: GPIO: Interupt request, channel 1
    GPIO50_IRQHandler,                 // 117: GPIO: Interupt request, channel 0
    GPIO51_IRQHandler,                 // 118: GPIO: Interupt request, channel 1
    GPIO60_IRQHandler,                 // 119: GPIO: Interupt request, channel 0
    GPIO61_IRQHandler,                 // 120: GPIO: Interupt request, channel 1
    GPIO70_IRQHandler,                 // 121: GPIO: Interupt request, channel 0
    GPIO71_IRQHandler,                 // 122: GPIO: Interupt request, channel 1
    PIN_INT0_IRQHandler,               // 123: PINT: Interupt request 0
    PIN_INT1_IRQHandler,               // 124: PINT: Interupt request 1
    PIN_INT2_IRQHandler,               // 125: PINT: Interupt request 2
    PIN_INT3_IRQHandler,               // 126: PINT: Interupt request 3
    PIN_INT4_IRQHandler,               // 127: PINT: Interupt request 4
    PIN_INT5_IRQHandler,               // 128: PINT: Interupt request 5
    PIN_INT6_IRQHandler,               // 129: PINT: Interupt request 6
    PIN_INT7_IRQHandler,               // 130: PINT: Interupt request 7
    SAI0_IRQHandler,                   // 131: SAI: TX/RX interrupt
    SAI1_IRQHandler,                   // 132: SAI: TX/RX interrupt
    SAI2_IRQHandler,                   // 133: SAI: TX/RX interrupt
    XSPI0_IRQHandler,                  // 134: xSPI: Ored interrupt
    XSPI1_IRQHandler,                  // 135: xSPI: Ored interrupt
    XSPI2_IRQHandler,                  // 136: xSPI: Ored interrupt
    MMU0_IRQHandler,                   // 137: MMU: Interrupt request
    MMU1_IRQHandler,                   // 138: MMU: Interrupt request
    MMU2_IRQHandler,                   // 139: MMU: Interrupt request
    Freqme_IRQHandler,                 // 140: FREQME: Interrupt request
    GDET0_IRQHandler,                  // 141: GDET0: Interrupt request
    Reserved142_IRQHandler,            // 142: Reserved interrupt
    Reserved143_IRQHandler,            // 143: Reserved interrupt
    GDET3_IRQHandler,                  // 144: GDET3: Interrupt request
    CDOG0_IRQHandler,                  // 145: CDOG: Interrupt request
    CDOG1_IRQHandler,                  // 146: CDOG: Interrupt request
    CDOG2_IRQHandler,                  // 147: CDOG: Interrupt request
    ITRC0_IRQHandler,                  // 148: ITRC: Interrupt, cleared by software
    ELS_IRQHandler,                    // 149: ELS Interrupt request
    PKC_IRQHandler,                    // 150: PKC: Public Key Cryptography Interrutp
    OSCCA_IRQHandler,                  // 151: OSCCA: OSCCA Interrupt request
    SLEEPCON0_IRQHandler,              // 152: WAKEUP: CPU0 wakeup event
    CPU0_IRQHandler,                   // 153: CTI0: interrupt request
    PMC_IRQHandler,                    // 154: PMC: Compute domain Interrupt
    JPEGDEC_IRQHandler,                // 155: JPEGDEC: Ored Context 0-3 interrupt request
    PNGDEC_IRQHandler,                 // 156: PGNDEC: Interrut request
    NPU_IRQHandler,                    // 157: NPU: Interrupt request
    TRNG_IRQHandler,                   // 158: TRNG: interrupt
    TEMPDET0_IRQHandler,               // 159: TEMPDET0: Interrupt request 0
    TEMPDET1_IRQHandler,               // 160: TEMPDET1: Interrupt request 1
    EZHV_IRQHandler,                   // 161: EZH-V: Interrupt request
    GLIKEY0_IRQHandler,                // 162: GLIKEY: Interrupt
    GLIKEY2_IRQHandler,                // 163: GLIKEY: Interrupt
    GLIKEY3_IRQHandler,                // 164: GLIKEY: Interrupt
    GLIKEY5_IRQHandler,                // 165: GLIKEY: Interrupt
    PVTS0_CPU0_IRQHandler,             // 166: PVTS0 CPU0 interrupt
    Reserved167_IRQHandler,            // 167: Reserved interrupt
    PVTS0_HIFI4_IRQHandler,            // 168: PVTS0 HIFI4 interrupt
    Reserved169_IRQHandler,            // 169: Reserved interrupt
    FRO0_IRQHandler,                   // 170: FRO: 300MHz FRO in VDD2 domain Async interrupt
    FRO1_IRQHandler,                   // 171: FRO: 192MHz FRO in VDD2 domain Async interrupt
    FRO2_IRQHandler,                   // 172: FRO: 300MHz FRO in VDD1 domain Async interrupt
    OCOTP_IRQHandler,                  // 173: OCOTP warning and integrity error interrupt
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

