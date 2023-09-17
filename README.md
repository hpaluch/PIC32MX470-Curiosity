# Getting started with PIC32MX470-Curiosity

> Latest news!
> As of 2023-09-15. It really beeps! Please
> see project No 2 below for details and also [mx470siren/](mx470siren)
> folder- it is not yet siren, because
> I'm totally exhausted, but someday it will be....
>
> There are few remaining issues described in chapter below

I recently acquired:

* [PIC32MX470-Curiosity DM320103][DM320103] development board

  ![PIC32MX470-Curiosity DM320103](assets/pic32mx470-curiosity-1280w.jpg)

* [PIC32 Audio Codec Daughter Board WM8904 AC328904][AC328904] codec
  board that uses pair of connectors known as X32

  ![PIC32 Audio Codec Daughter Board WM8904 ](assets/pic32-audio-codec-1024w.jpg)


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

This "warm up" project just tests (using PWM to divide 20 MHz High-Speed
resonator clock to 1 Mhz and outputs it on OC1 pin.

Please see dedicated [mx470osctest/](mx470osctest/)


## Project No. 2 - Siren

The goal is to produce two tone siren on Headphones Output of Audio Coded Daughter card. It is an adventurous attempt to reproduce project
for SAME70: https://github.com/Microchip-MPLAB-Harmony/audio/wiki/quick_start
- However there is important catch! We may not use Audio template to setup
  all components and their binding, because such Template is empty for
  PIC32MX

Status: It Beeps! (produces sine waveform on headphones output).

2023-09-17:
- it generally works (produces 1 kHz sine output on headphones),
  but there are two unresolved problems:

1. when new buffer is added on DMA there is spurious extra 0 Frame
   on sine waveform. Shown on picture below:

  ![I2S extra 0 frame](assets/wm8904/i2s/problems/scope-extra-0-frame.gif)

  - also provided `Digilent Analog Discovery 2` workspace file on:
  - [assets/wm8904/i2s/problems/MX470-Curiosity-siren-i2s-bclk-analog-trigger.dwf3work](assets/wm8904/i2s/problems/MX470-Curiosity-siren-i2s-bclk-analog-trigger.dwf3work)

2. Power Up Startup is wonky - after pushing MCLR/RESET button it works properly

3. There are noticeable clicks heard in headphones with frequency
   similar to DMA switches - however I'm unable to spot them on scope
   (I think that this extra 0 in sine does not produce such click sound)

Buffer debug info:
```
app.c:124 data: 0,4276,...,-8480,-4276
app.c:126 bufsize=38400
```

The 38400 is buffer size in bytes, to get number of samples divide it
by 4 (because there are 2 bytes for left channel and 2 bytes for right channel,
per sample): 9600


```
channel=0
srcAddr=0xA0009960
srcSize=0x00009600
destAddr=0xBF805A20
destSize=2
cellSize=2
```

When ratio is 4, DMA parameters are exactly same(!)

> WARNING!
>
> MCC Harmony tool sometimes screws I2S0 instance component, removing
> required sources on Generate action. The remedy
> is to always check:
> - Project Graph -> I2S Driver -> click on `Instance 0`.
> - if `Configuration Options` tab  is empty (only `I2S` word is shown), you
>   are in trouble
> - in such case Do NOT click on Generate, but close and open MCC
>   Harmony - it usually helps(!)


2023-09-15
- Got it working! Not sure abto
  - Most important discovery: 
    - when WM8904 is set to be Slave
    - I2S2 MUST be Master ! (opposite)
  - because Plib somehow does not like 

Today looking at I2S clock issue (no clock output).
- fixed MCLK, BCLK and LRCLK - need to verify remainder of things
- using debug message got:
  ```
  _samplingRateSet:1814 bitClk=1,536,000 mclk=12,288,000
  ```
- where `mclk = bitClk * 8`
- and 1,536,000 / 32 = 48,000 (our sample rate)


- generated code is expecting internal MCLK signal from REFCLOKO, which was not yet
  entable...


Stored analysis data are under: 
- [assets/wm8904/i2c/](assets/wm8904/i2c/)
- where:
  - `*.dwf3work` - I2C capture Workspace for `Digilent Analog Discovery 2`, software
    `Digilent WaveForms` Workspace v3.20.1
  - `*.ods` - spreadsheet in LibreOffice 7.5 format
- WARNING! These data are very incomplete as can be compared with debug messages...


- finished import of skeleton from: https://github.com/Microchip-MPLAB-Harmony/audio/wiki/quick_start + many
  fixes.
- there is one task, that copies S1 button state to LED3
- there is Debug console output on UART1 (see below for more info)
- on startup it produces output:
  ```
  Starting ../src/app.c:143 - Codec App v1.00
  ../src/app.c:148 Finished APP_Initialize().
  ../src/app.c:171 Initializing codec...
  ../src../src/app.c:189 OK: Codec initialized
  ../src/app.c:202 Setting Up codec buffers...
  ../src/app.c:216 Playing sound (Buffer ping-pong)...
  ```
- the wrong src line is probably from overflown ring buffer... or some kind of thread race with other app

Current diagnostics:
- soldered J9 and J11 on PIC32MC470 Curiosity to get access to second I2C (Codec control)
  and some SPI2/I2S2 signals (but not all)
- I2C with Digilent Analog Discovery 2
- Init sequence: 0x1A write 0x00 ACK, 0xFF ACK,0xFF ACK, STOP


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


Additional Peripherals:
- Pin 12, RB4, mikroBus 1, J5, pin 1 "AN" - trigger for scope when transfer starts
- RGB LED:
  - Lits RED on general exception forever
  - Lits Magenta on bootstrap exception forever
- Application #2: Copies state of Button S1 to LED3.
- Console output redirected to UART1, using J5 mikroBus1 connector
  RX (Input to PIC) and TX (Output from PIC)
  - connect using any cand of 3.3V TTL UART cable to USB, and set 115200 Baud, 8-bit data, no-parity, no flow-control

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
