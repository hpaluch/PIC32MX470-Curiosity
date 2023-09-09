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
- it is MIP32 M4K CPU CORE
  - [MIPS32 M4K Manual][MIPS32 M4K Manual] from mips.com
  - [MIPS32 M4K Core Datasheet][MIPS32 M4K DTS] from mips.com
  - [MIPS32 Instruction Set][MIPS32 BIS] from mips.com
  - [MIPS32 Instruction Set Quick Reference][MIPS32 QRC] from mips.com

# Hardware setup

Ensure that Jumpers are in these default positions -
see [PIC32MX470 Curiosity Development Board User’s Guide][DS70005283B]
for details:

Power Jumper J8:
- middle position, left label `PKOB` (PicKit on board debugger)

External/internal Debugger Jumper J2:
- Pins 1-2 and 3-4 shorted (using single rectangular Jumper)

Connecting board to USB:
- use `USB mini-B (J3)`
- it is located at Left BOTTOM of board (!)
- there is also white label `USB DEBUG`
- NOTE: That Left-top micro-USB connector is for USB connection
  to PIC32MX! (that can work both as Host and/or as Device)

# Available peripherals

Extracted from
manual [PIC32MX470 Curiosity Development Board User’s Guide][DS70005283B].

- CPU is U9 is `PIC32MX470F512H-120/PT`,
  - package: code `PT` => 64-pin, TQFP (visible pins)
- X2 20 MHz crystal oscillator -  this precision clock source
  is required when using USB on PIC32MX CPU

| PIC32MX Pin | PIC32MX Function | Peripheral |
| --- | --- | --- |
| 7 | /MCLR | RESET Button /MCLR (between USB connectors) |
| 54 | RD6   | Switch S1 |
| 64 | RE4 | LED1 red - active high |
| 2 | RE6 | LED2 yellow - active high |
| 3 | RE7 | LED3 green - active high |
| 23 | RB10 | RGB LED4 - red - active low |
| 13 | RB3 | RGB LED4- green - active low |
| 14 | RB2 | RGB LED4 - blue - active low |
| 39 | OSC1 | 20 MHz crystal - CPU osc input |
| 40 | OSC2 | 20 MHz crystal - CPU osc output | 

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
