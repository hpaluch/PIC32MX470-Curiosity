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


