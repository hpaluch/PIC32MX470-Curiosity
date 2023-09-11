/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

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

#include "app.h"
// see https://github.com/Microchip-MPLAB-Harmony/audio/wiki/quick_start
#include <math.h>   // for sin & M_PI

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define APP_VERSION 101 // 123 = 1.23

// like __FILE__ without path, using const to avoid duplication on each use.
static const char *APP_FILE = "app.c";

// audio buffers
DRV_I2S_DATA16 __attribute__ ((aligned (32))) 
    App_Tone_Lookup_Table_tone1[APP_MAX_AUDIO_NUM_SAMPLES];
DRV_I2S_DATA16 __attribute__ ((aligned (32))) 
    App_Tone_Lookup_Table_tone2[APP_MAX_AUDIO_NUM_SAMPLES];

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
void _codecTxBufferComplete()
{
    //Next State -- after the buffer complete interrupt.
    appData.state = APP_STATE_CODEC_ADD_BUFFER;    
    LED2_Toggle();
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

uint32_t _sineTableInit(DRV_I2S_DATA16* buffer, uint32_t maxBufferSize,
	uint32_t frequency, uint32_t sampleRate)
{
    uint32_t i,j,k, maxNumCycles;
    uint32_t numSamplesPerCycle;

    // generate sine table
    // # of samples for one cycle, e.g. 48 for 1 kHz @ 48,000 samples/sec   
    numSamplesPerCycle = sampleRate / frequency;
    
    // max # of cycles we can fit in buffer
    maxNumCycles = maxBufferSize / numSamplesPerCycle; 
    
    k = 0;
    for (j=0; j < maxNumCycles; j++)
    {
        for (i=0; i < numSamplesPerCycle; i++)
        {
            // radians = degrees × pi / 180°            
            double radians = (M_PI*(double)(360.0/
               (double)numSamplesPerCycle)*(double)i)/180.0;

            buffer[i+k].leftData = (int16_t)(0x7FFF*sin(radians));          
            buffer[i+k].rightData = buffer[i+k].leftData;        
        }
        k += numSamplesPerCycle;
    }
    
    return sizeof(DRV_I2S_DATA16)*k;    // return size of filled-in buffer   
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
void _audioCodecInitialize (AUDIO_CODEC_DATA* codecData)
{
    codecData->handle = DRV_HANDLE_INVALID;
    codecData->context = 0;
    codecData->bufferHandler = 
           (DRV_CODEC_BUFFER_EVENT_HANDLER) _codecTxBufferComplete;
    codecData->txbufferObject1 = (uint8_t *) App_Tone_Lookup_Table_tone1;
    codecData->txbufferObject2 = (uint8_t *) App_Tone_Lookup_Table_tone2;
    codecData->bufferSize1 = 0;
    codecData->bufferSize2 = 0;
}


/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Initialize ( void )
{
    SYS_CONSOLE_PRINT("%s:%d Codec App v%d.%02d\r\n",
            APP_FILE,__LINE__,APP_VERSION/100,APP_VERSION%100);
    appData.state = APP_STATE_CODEC_OPEN;
    _audioCodecInitialize (&appData.codecData);
    appData.frequency = APP_FREQUENCY;      // e.g. 1 kHz
    appData.pingPong = 1;
    SYS_CONSOLE_PRINT("%s:%d Done. %s().\r\n", APP_FILE, __LINE__, __func__);
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks ( void )
{
    static bool printed = false;
    
    // Check the application's current state
    switch ( appData.state )
    {       
        // Application's initial state 
        case APP_STATE_CODEC_OPEN:
        {
            if(!printed){
                printed=true;
                SYS_CONSOLE_PRINT("%s:%d S1\r\n",
                        APP_FILE,__LINE__);
            }
            // See if codec is done initializing
            SYS_STATUS status = DRV_CODEC_Status(sysObjdrvCodec0);
            if (SYS_STATUS_READY == status)
            {
                // This means the driver can now be opened.
                // Open the driver object to get a handle
                appData.codecData.handle = DRV_CODEC_Open(DRV_CODEC_INDEX_0, 
                    DRV_IO_INTENT_WRITE | DRV_IO_INTENT_EXCLUSIVE);       
                if(appData.codecData.handle != DRV_HANDLE_INVALID)
                {
                    // Get sampling rate set up in MHC (e.g. 48000)
                    appData.sampleRate =
                        DRV_CODEC_SamplingRateGet(appData.codecData.handle);
                       
                    appData.state = APP_STATE_CODEC_SET_BUFFER_HANDLER;
                }  else {
                    SYS_DEBUG_PRINT(SYS_ERROR_FATAL,"%s:%d DRV_CODEC_Open() failed!\r\n",
                            APP_FILE,__LINE__);
                }          
            }
        }
        break;
        
        // Set a handler for the audio buffer completion event
        case APP_STATE_CODEC_SET_BUFFER_HANDLER:
        {
            SYS_CONSOLE_PRINT("%s:%d S2\r\n",
                        APP_FILE,__LINE__);
            DRV_CODEC_BufferEventHandlerSet(appData.codecData.handle, 
                                            appData.codecData.bufferHandler, 
                                            appData.codecData.context);                                 
               
            // Initialize the first buffer with sine wave data
            appData.codecData.bufferSize1 = _sineTableInit(
                (DRV_I2S_DATA16*)appData.codecData.txbufferObject1,
                APP_MAX_AUDIO_NUM_SAMPLES, appData.frequency,
				appData.sampleRate);
                        
            appData.pingPong = 1;   // indicate buffer 1 to be used first 
            appData.state = APP_STATE_CODEC_ADD_BUFFER;           
        }
        break;
       
        case APP_STATE_CODEC_ADD_BUFFER:
        {
            // Will ping-pong back and forth between buffer 1 and 2                      
            if (appData.pingPong==1)
            {
                // hand off first buffer to DMA
                DRV_CODEC_BufferAddWrite(appData.codecData.handle,
                    &appData.codecData.writeBufHandle1,
                    appData.codecData.txbufferObject1,
                    appData.codecData.bufferSize1);

                if (appData.codecData.writeBufHandle1 != 
                    DRV_CODEC_BUFFER_HANDLE_INVALID)
                {   
                    // just sent buffer 1 to DMA,
                    // so fill in buffer 2 for next time
                    appData.codecData.bufferSize2 = _sineTableInit(
                        (DRV_I2S_DATA16*)appData.codecData.txbufferObject2,
                        APP_MAX_AUDIO_NUM_SAMPLES, appData.frequency,
						appData.sampleRate);

                    appData.pingPong = 2;		// switch to second buffer
                    appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
                } else {
                    SYS_DEBUG_PRINT(SYS_ERROR_FATAL,"ERROR: %s:%d DRV_CODEC_BufferAddWrite() failed!\r\n",
                            APP_FILE,__LINE__);                    
                }
            }
            else
            {
                // hand off second buffer to DMA                
                DRV_CODEC_BufferAddWrite(appData.codecData.handle,
                        &appData.codecData.writeBufHandle2, 
                        appData.codecData.txbufferObject2,
                        appData.codecData.bufferSize2);

                if (appData.codecData.writeBufHandle2 != 
                    DRV_CODEC_BUFFER_HANDLE_INVALID)
                {
                    // just sent buffer 2 to DMA,
                    // so fill in buffer 1 for next time
                    appData.codecData.bufferSize1 = _sineTableInit(
                        (DRV_I2S_DATA16*)appData.codecData.txbufferObject1,
                        APP_MAX_AUDIO_NUM_SAMPLES, appData.frequency,
						appData.sampleRate);
             
                    appData.pingPong = 1;		// back to first buffer
                    appData.state = APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE;
                } else {
                    SYS_DEBUG_PRINT(SYS_ERROR_FATAL,"ERROR: %s:%d DRV_CODEC_BufferAddWrite() failed!\r\n",
                        APP_FILE,__LINE__);
                }
            }                       
        }
        break;

        // Audio data Transmission under process, wait for callback
        case APP_STATE_CODEC_WAIT_FOR_BUFFER_COMPLETE:
        {
        }
        break;
         
        default:
        {
            /* TODO: Handle error in application's state machine. */
        }
        break;             
    }
}


/*******************************************************************************
 End of File
 */
