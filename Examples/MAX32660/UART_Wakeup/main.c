/**
 * @file        main.c
 * @brief       LP Serial Character Wake Up Example
 * @details     
 */

/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. All Rights Reserved.
 * (now owned by Analog Devices, Inc.),
 * Copyright (C) 2023 Analog Devices, Inc. All Rights Reserved. This software
 * is proprietary to Analog Devices, Inc. and its licensors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "mxc_pins.h"
#include "nvic_table.h"
#include "led.h"
#include "pb.h"
#include "board.h"
#include "gpio.h"
#include "lp.h"
#include "uart.h"

/***** Definitions *****/
#define RXBUF_SIZE 100

/***** Globals *****/
mxc_uart_regs_t *ConsoleUART = MXC_UART_GET_UART(CONSOLE_UART);
char rxBuf[RXBUF_SIZE];
volatile int cnt = 0;
volatile bool crRecv = false;

/***** Functions *****/
void UART1_Handler(void)
{
    // To measure wake up time, probe P0.13 (LED Pin).
    LED_On(0);

    if (ConsoleUART->int_fl & MXC_F_UART_INT_FL_RX_FIFO_LVL) {
        ConsoleUART->int_fl |= MXC_F_UART_INT_FL_RX_FIFO_LVL;

        while ((ConsoleUART->stat &
                MXC_F_UART_STAT_RX_NUM)) { //Continue to read characters until receive buffer empty
            if (cnt >= RXBUF_SIZE) { //Prevent buffer overflow
                cnt = 0;
            } else {
                rxBuf[cnt] = (char)MXC_UART_ReadCharacter(ConsoleUART); //Read character
                if (rxBuf[cnt] == '\r') { //Last character received?
                    crRecv = true;
                }
                cnt++;
            }
        }
    }
}

int main(void)
{
    memset(rxBuf, 0x0, RXBUF_SIZE * sizeof(char));

    printf("\n\n******************** LP Serial Character Wake Up Example *******************\n\n");
    printf("This example demonstrates how to send a serial character to wake up the device.\n");
    printf("Each string sent will be echoed to the terminal. Strings sent to wake up the\n");
    printf("device will appear as garbage due to wake-up latency, each of the following\n");
    printf("strings will be processed correctly. Sending \"sleep\" will put the device\n");
    printf("back in deep sleep and sending \"quit\" will end the example.\n");
    printf("\nTo measure wake-up latency, probe pins P0.13 (LED) and P0.11 (UART RX Pin).\n");
    printf("\n**NOTE**: Each string sent to the device must end in a \"\\r\" character for the\n");
    printf("strings to be processed correctly.\n");

    printf("\nPress PB1 to begin the demo.\n");
    while (!PB_Get(0)) {}

    /* Configure serial character interrupts */
    NVIC_ClearPendingIRQ(UART1_IRQn);
    NVIC_DisableIRQ(UART1_IRQn);
    MXC_NVIC_SetVector(UART1_IRQn, UART1_Handler);
    NVIC_EnableIRQ(UART1_IRQn);
    MXC_UART_SetRXThreshold(ConsoleUART, 1);
    MXC_UART_EnableInt(ConsoleUART, MXC_F_UART_INT_EN_RX_FIFO_LVL);

    /* Configure UART RX (P0.11) as wake up pin */
    MXC_LP_EnableGPIOWakeup(&gpio_cfg_uart1a);

    /* Put device in DeepSleep operating mode */
    printf("Now entering deep sleep mode. Send any character string to wake up the device.\n\n");
    while (MXC_UART_Busy(ConsoleUART)) {}
    MXC_LP_ClearWakeStatus();
    MXC_LP_EnterDeepSleepMode();

    while (1) {
        if (crRecv) {
            printf("String Received: %s\n",
                   rxBuf); //Print character string received from the console

            if (!strcmp(rxBuf, "sleep\r")) { //If "sleep\r" received, go back to sleep
                printf("Going back to deep sleep.\n");
                while (MXC_UART_Busy(ConsoleUART)) {}
                LED_Off(0);
                MXC_LP_ClearWakeStatus();
                MXC_LP_EnterDeepSleepMode();
            } else if (!strcmp(rxBuf, "quit\r")) { //If "quit\r" received, end example.
                break;
            }

            crRecv = false;
            cnt = 0;
            memset(rxBuf, 0x0, RXBUF_SIZE * sizeof(char));
        }
    }

    printf("Example complete!");
    while (MXC_UART_Busy(ConsoleUART)) {}

    return 0;
}
