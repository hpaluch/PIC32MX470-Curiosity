/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_s1led.c

  Summary:
    Simple application that just copies S1 switch status to LED3

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app_s1led.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
// like __FILE__ without path, using const to avoid duplication on each use.
static const char *APP_FILE = "app_s1led.c";

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_S1LED_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_S1LED_DATA app_s1ledData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
*/
static void AppLed_TimerCallback( 
                uintptr_t context )
{
        // APP_S1LED_DATA *appData = (APP_S1LED_DATA*)context;
    GPIO_RD2_Toggle(); // mikroBus1 SCK
        
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_S1LED_Initialize ( void )

  Remarks:
    See prototype in app_s1led.h.
 */
void APP_S1LED_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_s1ledData.state = APP_S1LED_STATE_INIT;
    app_s1ledData.tmrHandle = SYS_TIME_CallbackRegisterMS(AppLed_TimerCallback, 
                    (uintptr_t)&app_s1ledData, 1/*ms*/, SYS_TIME_PERIODIC);
    if (app_s1ledData.tmrHandle == SYS_TIME_HANDLE_INVALID){
        SYS_DEBUG_PRINT(SYS_ERROR_FATAL,"ERROR: %s:%d SYS_TIME_CallbackRegisterMS() failed!\r\n",
               APP_FILE,__LINE__);
        app_s1ledData.state = APP_S1LED_STATE_FATAL_ERROR;
    }
}

/******************************************************************************
  Function:
    void APP_S1LED_Tasks ( void )

  Remarks:
    See prototype in app_s1led.h.
 */
void APP_S1LED_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app_s1ledData.state )
    {
        /* Application's initial state. */
        case APP_S1LED_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {
                SYS_CONSOLE_PRINT("%s:%d started.\r\n",APP_FILE,__LINE__);
                app_s1ledData.state = APP_S1LED_STATE_SERVICE_TASKS;
            }
            break;
        }

        // we always do single task, copy S1 switch to LED3
        case APP_S1LED_STATE_SERVICE_TASKS:
        {
            // Please note that S1 is shorting to ground, so
            // open switch = active LED
            if (SWITCH1_Get()){
                LED3_On();
            } else {
                LED3_Off();
            }
            break;
        }

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
