;------------------------------------------------------------------------------
;
; Copyright (C) 2022-2023 Maxim Integrated Products, Inc. All Rights Reserved.
; (now owned by Analog Devices, Inc.),
; Copyright (C) 2023 Analog Devices, Inc. All Rights Reserved. This software
; is proprietary to Analog Devices, Inc. and its licensors.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;     http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.
;
;------------------------------------------------------------------------------

    DCD     Fake17_IRQHandler           ; 17:01 -- No IRQ assignments yet */
    DCD     Fake18_IRQHandler           ; 18:01 -- No IRQ assignments yet */
    ; Continue this pattern when vectors are eventually assigned by hardware

__Vectors_End
__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors



    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
    THUMB
    PUBWEAK Reset_Handler
    SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler

        LDR        R0, =SystemInit
        BLX        R0
        LDR        R0, =__iar_program_start
        BX         R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK Fake16_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
Fake16_IRQHandler
        B       Fake16_IRQHandler

        PUBWEAK Fake17_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
Fake17_IRQHandler
        B       Fake17_IRQHandler

        PUBWEAK Fake18_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
Fake18_IRQHandler
        B       Fake18_IRQHandler

        END
