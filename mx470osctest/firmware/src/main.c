/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes


volatile uint32_t gRgbStage = 0;

// Core Timer Interrupt Handler (1 second rate)
void CORETIMER_EventHandler(uint32_t status, uintptr_t context)
{
    // ROTATE RGB LED in 4 cycles: OFF -> R -> G -> B
    switch(gRgbStage){
        case 0:
            // ensure that RGB LED is really off
            RGB_LED_RED_Off(); RGB_LED_GREEN_Off(); RGB_LED_BLUE_Off();
            break;
        case 1:
            // Red ON
            RGB_LED_RED_On();
            break;
        case 2:
            // Red Off, Green On
            RGB_LED_RED_Off(); RGB_LED_GREEN_On();
            break;
        case 3:
            // Green Off, Blue On
            RGB_LED_GREEN_Off(); RGB_LED_BLUE_On();
            break;
    }
    
    gRgbStage = (gRgbStage+1) & 3; // limit values to 0,1,2,3,0,1,2,3,...
} 

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    uint32_t i = 0;
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    CORETIMER_CallbackSet(CORETIMER_EventHandler,(uintptr_t)NULL);
    CORETIMER_Start();
    OCMP1_Enable(); // enable PWM toggle, mikroBUS 1, pin PWM
    TMR2_Start(); // Timer2 is base for PWM (OC1)
    while ( true )
    {
        // rotate: OFF -> LED1 -> LED2 -> LED3 at 0.5s rate
        for(i=0;i<4;i++){
            switch(i){
                case 0:
                    // ensure that all 3 LEDs are off
                    LED1_Off(); LED2_Off(); LED3_Off();
                    break;
                case 1:
                    LED1_On();
                    break;
                case 2:
                    LED1_Off(); LED2_On();
                    break;
                case 3:
                    LED2_Off(); LED3_On();
                    break;
            }
            CORETIMER_DelayMs(500);
        }
        
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        // Unused so far...
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/
