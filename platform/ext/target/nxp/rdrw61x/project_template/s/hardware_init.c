/*
 * Copyright 2018, 2022, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "app.h"
#include "fsl_flexspi.h"
#include "flash_layout.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Flexspi);
    RESET_ClearPeripheralReset(kFLEXSPI_RST_SHIFT_RSTn);
}

void SystemInitHook(void)
{
    extern void *__VECTOR_TABLE[];
    SCB->VTOR = (uint32_t) & (__VECTOR_TABLE[0]);

    /* Access for NS part */
    /* Coprocessor Access Control Register */
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    SCB_NS->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10, CP11 Full Access in Non-secure mode */
#endif                                                    /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

    SCB_NS->CPACR |=
        ((3UL << 0 * 2) | (3UL << 1 * 2)); /* set CP0, CP1 Full Access in Non-secure mode (enable PowerQuad) */
}

uint32_t USART3_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(3U);
}
/*${function:end}*/
