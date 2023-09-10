# Getting started with PIC32MX470-Curiosity

I recently acquired:

* [PIC32MX470-Curiosity DM320103][DM320103] development board

  ![PIC32MX470-Curiosity DM320103](assets/pic32mx470-curiosity-1280w.jpg)

* [PIC32 Audio Codec Daughter Board WM8904 AC328904][AC328904] codec
  board that uses pair of connectors known as X32

  ![PIC32 Audio Codec Daughter Board WM8904 ](assets/pic32-audio-codec-1024w.jpg)

So I will have lot of fun playing with Codec, DSPs...

> WARNING! Official recommended codec for this card 
> is [PIC32 Audio Codec Daughter Board - AK4642EN][AC320100]
> but it is harder to get. So all codec stuff or recommendations
> are WITHOUT ANY WARRANTY!

However I need somehow to start with some introductory projects.

# Basic information

Most important links regarding Curiosity board:
* Official start URL for [PIC32MX470-Curiosity DM320103][DM320103] 
* [PIC32MX470 Curiosity Development Board User’s Guide][DS70005283B]


Links for CPU [PIC32MX470F512H][PIC32MX470F512H]
- datasheet [PIC32MX470F512H - DS60001185H][DS60001185H]
- errata - IMPORTANT!: [DS80000574][DS80000574]
- it is `MIPS32(R) M4K` CPU Core.
  - [MIPS32 M4K Manual][MIPS32 M4K Manual] from mips.com
  - [MIPS32 M4K Core Datasheet][MIPS32 M4K DTS] from mips.com
  - [MIPS32 Instruction Set][MIPS32 BIS] from mips.com
  - [MIPS32 Instruction Set Quick Reference][MIPS32 QRC] from mips.com

# Hardware setup

Ensure that Jumpers are in these default positions -
see [PIC32MX470 Curiosity Development Board User’s Guide][DS70005283B]
for details:

Power Jumper `J8`:
- middle position, left label `PKOB` (PicKit on board debugger)

External/internal Debugger Jumper `J2`:
- Pins 1-2 and 3-4 shorted (using single rectangular Jumper)

Connecting board to USB:
- use `USB mini-B (J3)`
- it is located at Left BOTTOM of board (!)
- there is also white label `USB DEBUG`
- NOTE: That Left-top micro-USB connector is for USB connection
  to PIC32MX! (that can work both as Host and/or as Device) using recent
  USB OTG (On-The-Go) protocol.

# Available peripherals

Extracted from
manual [PIC32MX470 Curiosity Development Board User’s Guide][DS70005283B].

- CPU is `U9` part `PIC32MX470F512H-120/PT`,
  - package: code `PT` => 64-pin, TQFP (visible pins)
  - Note: it is important to know proper package when using
    Pin-Manager tool in MCC Harmony tool
- X2 20 MHz crystal oscillator -  this precision clock source
  is required when using USB on PIC32MX CPU and should be also
  used when working with Audio Codec that is sensitive to any kind
  of jitter. It must be configured on this PIC as `Primary HS` (Primary
  High Speed oscillator).

To see known peripheral you can also look at Board Support Package (BSP)
- at: https://github.com/Microchip-MPLAB-Harmony/bsp/blob/master/boards/pic32mx470_curiosity/config/bsp.py

Here is my curated list:

| PIC32MX Pin | PIC32MX Function | Peripheral | Active level (0/1) |
| ---: | --- | --- | --- |
| 7 | /MCLR | RESET Button /MCLR (between USB connectors) | 0 |
| 54 | RD6   | Switch S1 | 0 |
| 64 | RE4 | LED1 red | 1 |
| 2 | RE6 | LED2 yellow | 1 |
| 3 | RE7 | LED3 green | 1 |
| 23 | RB10 | RGB LED4 - red | 0 |
| 13 | RB3 | RGB LED4- green | 0 |
| 14 | RB2 | RGB LED4 - blue | 0 |
| 39 | OSC1 | 20 MHz crystal - CPU osc input | - |
| 40 | OSC2 | 20 MHz crystal - CPU osc output | - |

# Software requirements

Currently all project use:
- MPLAB X IDE v6.15 (latest version as of Sep 2023)
- XC32 v4.30 (latest version as of Sep 2023)
- MCC Harmony generator

Common project properties:
- on TRAP, RGB LED should permanently lit RED, and all 3 LED1, LED2 and LED3 should be lit.

# Project List


## Project1 - 20 MHz Oscillator test


The Curiosity board contains high-precision 20 MHz resonator. It is required for USB
support and other high-precision timing projects.

The mikroBUS 1 Pin PWM can be used to check for proper frequency (1 MHz). The output
frequency on this pin is generated entirely in hardware without interrupt using:
- Timer2 as base for Output Compare 1 (OC1) in PWM mode, Period is 10, so Timer overflows
  with 20 Mhz / 10 = 2 MHz frequency
- Output Compare 1 has period 5 (does not matter much). And on each match it toggles
  output. Therefore OC1 output is 2 MHz /2 = 1 MHz.
- OC1 Output is connected to mikroBUS 1 (J5), Pin PWM

WARNING! You should never enable Timer2 interrupts on such high frequency (2 MHz in case of
Timer2) - MIPS32 have very high interrupt overhead, because it has to save and restore
more than 32 registers on stack. Higher MIPS CPUs offers SRS (Shadow Register Sets) to reduce
overhead, but it is not our case of PIC32MX.

I intentionally used 1 MHz output because:
- it is high enough to detect any kind on jitter
- it is low enough that it can be viewed and/or measured using common consumer grade
  scopes (in my case `Digilent Analog Discovery 2`)

Additionally this project rotates both RGB LED components and LED1,2,3 to verify
that Core Timer interrupt works properly and that Core Timer polling works properly:

1. Core Timer interrupt (to verify that Timer interrupts works). It rotates colors
   on RGB LED using sequence: Off, Red, Green, Blue  - change every 1 s
2. Using Detail in main loop (to verify that main program thread is working properly - for example
   it is not clogged with interrupt storm). It uses `CORETIMER_DelayMs(500);` that polls 
   Core Timer (without interrupt). It rotated LED1, LED2, LED3 in sequence: Off, LED1, LED2, LED3
   every 0.5 s (2 times faster that RGB LED, however the actual rate is slightly over 0.5s, because
   we don't compensate for code overhead).

This project was generated using:
- MCC Harmony plugin
- Used Optional package: `Harmony 3 - Core` -> `BSP` (Board Support Package)
  - according to URL: 
    - at: https://github.com/Microchip-MPLAB-Harmony/bsp/blob/master/boards/pic32mx470_curiosity/config/bsp.py
  - this BSP should includes our `MX470 Curiosity` board.

Additional I/O:

| CPU Pin | Name | Function | Board Pin |
| ---: | --- | --- | --- |
| 29 | RPB14 | OC1 | J5 Pin 16 - MikroBUS1 - Pin PWM |

So on MikroBUS 1 PWM pin there should be rectangular 1 MHz frequency (oscillator 20 MHz / 20 )
using 3.3V TTL logic.

## Project No. 2 - Siren

The goal is to produce two tone siren on Headphones Output of Audio Coded Daughter card.

Status: Work in Progress

Required hardware:
* [PIC32MX470-Curiosity DM320103][DM320103] development board
* [PIC32 Audio Codec Daughter Board WM8904 AC328904][AC328904] inserted on X32 connector of
  development board

NOTE: All Jumpers on Audio card (J6,J7,J8,J9) should be on right position
(where is small white circle) to use I2S1 signals (only these are connected on
MX470 Curiosity board)
 
Important information on Audio Codec Daughter card:
- codec [WM8904][WM8904] and [WM8904 Datasheet][WM8904DS]


| CPU pin | Curiosity Pin J14 | Curiosity Signal J14 | Board signal | WM8904 Pin | WM8904 Signal |
| ---: | ---: | --- | --- | ---: | --- |
| X | 6 | RPD0/INT0 | `UART1_RTS#` | 1 | IRQ/GPIO1 |
| 32 | 7 | RPF5/SCL2 | SCL1 | 2 | SCL |
| 31 | 9 | RPF4/SDA2 | SDA1 | 3 | SDA |
| 8 | 10 | RPG9/SS2 (out) | `I2S1_RCL` | 30 | LRCLK |
| 5 | 11 | RPG7/SDI2 | `I2S1_MISO` | 31 | ADCDAT |
| 4 | 12 | RPG6/SCK2 | `I2S1_BCLK` | 29 | BCLK |
| 6 | 13 | RPG8/SDO2 | `I2S1_MOSI` | 32 | DACDAT |
| 21 | 14 | RPB8/REFCLKO | `I2S1_MCLK` | MCLK |

Regarding MCC Harmony code generator - there is Audio Codec Library
for WM8904 on:
- https://github.com/Microchip-MPLAB-Harmony/audio/tree/master/driver/codec/WM8904
- and here is even Example for SAM Atmel: [Harmony WM8904 Example][Harmony WM8904 Example]

This does not work (probably PIC470MX not supported):
- Device Resources -> Libraries -> Harmony -> Audio -> Templates -> WM8904 Codec

Tips:

How to resolve error - missing include `system/dma/sys_dma.h`
- invoke MCC
- select `Project Graph` tab
- click on `Core / Harmony Core Service`
- Enable checkbox `Enable System DMA`
- click on `Generate ...`

Once you resolve this error you will get another one related to `SYS_TIME...`
- to fix it again invoke MCC
- add Device Resources -> Libraries -> Harmony -> System
  Services -> TIME
- you need to also add TMR provider
- so also add Device Resources -> Libraies ->
  Harmony -> Peripherals -> CORE TIMER ->  CORE TIEMR
  - keep Comapre period 1ms (not sure if it is not too much)
  - Do NOT touch any other settings! They will be modified automatically in next step!
- now on Project Graph  connect CORE TIMER -> TMR to TIME System Service -> TMR

This time I got following error:
```
../src/config/default/audio/peripheral/i2s/plib_i2s2.c: In function 'I2S2_LRCLK_Get':
../src/config/default/audio/peripheral/i2s/plib_i2s2.c:114:31: \
  error: 'PORTA' undeclared (first use in this function); did you mean 'PORTG'?
     volatile uint32_t ret = ((PORTA >> 0) & 0x1);
```



[Harmony WM8904 Example]: https://github.com/Microchip-MPLAB-Harmony/audio/wiki/quick_start
[WM8904DS]: https://statics.cirrus.com/pubs/proDatasheet/WM8904_Rev4.1.pdf
[WM8904]: https://www.cirrus.com/products/wm8904/ 
[MIPS32 M4K DTS]: https://s3-eu-west-1.amazonaws.com/downloads-mips/documents/MD00247-2B-M4K-DTS-02.01.pdf
[MIPS32 BIS]: https://s3-eu-west-1.amazonaws.com/downloads-mips/documents/MD00086-2B-MIPS32BIS-AFP-05.04.pdf
[MIPS32 QRC]: https://s3-eu-west-1.amazonaws.com/downloads-mips/documents/MD00565-2B-MIPS32-QRC-01.01.pdf
[MIPS32 M4K Manual]: https://s3-eu-west-1.amazonaws.com/downloads-mips/documents/MD00249-2B-M4K-SUM-02.03.pdf

[PIC32MX470F512H]: https://www.microchip.com/en-us/product/pic32mx470f512h
[DS60001185H]: https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/DataSheets/PIC32MX330350370430450470_Datasheet_DS60001185H.pdf
[DS80000574]: https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/Errata/PIC32MX-330-350-370-430-450-470-Family-Silicon-Errata-and-Data-Sheet-Clarification-DS80000574.pdf
[DS70005283B]: https://ww1.microchip.com/downloads/en/DeviceDoc/70005283B.pdf
[DM320103]: https://www.microchip.com/en-us/development-tool/dm320103
[AC328904]: https://www.microchip.com/en-us/development-tool/ac328904
[AC320100]: https://www.microchip.com/en-us/development-tool/ac320100
