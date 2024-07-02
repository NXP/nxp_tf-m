/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016, 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
}

void SystemInit(void)
{
}

uint32_t USART3_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(3U);
}
/*${function:end}*/
