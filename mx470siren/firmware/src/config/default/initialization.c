/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "definitions.h"
#include "device.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************

/*** DEVCFG0 ***/
#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx2
#pragma config PWP =        OFF
#pragma config BWP =        OFF
#pragma config CP =         OFF


/*** DEVCFG1 ***/
#pragma config FNOSC =      PRIPLL
#pragma config FPBDIV =     DIV_1
#pragma config FSOSCEN =    OFF
#pragma config IESO =       ON
#pragma config POSCMOD =    HS
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSDCMD
#pragma config WDTPS =      PS1048576
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     OFF
#pragma config FWDTWINSZ =  WINSZ_25


/*** DEVCFG2 ***/
#pragma config FPLLIDIV =   DIV_5
#pragma config FPLLMUL =    MUL_24
#pragma config FPLLODIV =   DIV_2
#pragma config UPLLEN =     OFF
#pragma config UPLLIDIV =   DIV_2

/*** DEVCFG3 ***/
#pragma config FSRSSEL =    PRIORITY_7
#pragma config FVBUSONIO =  ON
#pragma config USERID =     0xffff
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO =   ON





// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
/* Following MISRA-C rules are deviated in the below code block */
/* MISRA C-2012 Rule 11.1 */
/* MISRA C-2012 Rule 11.3 */
/* MISRA C-2012 Rule 11.8 */
// <editor-fold defaultstate="collapsed" desc="DRV_I2C Instance 0 Initialization Data">

/* I2C Client Objects Pool */
static DRV_I2C_CLIENT_OBJ drvI2C0ClientObjPool[DRV_I2C_CLIENTS_NUMBER_IDX0];

/* I2C Transfer Objects Pool */
static DRV_I2C_TRANSFER_OBJ drvI2C0TransferObj[DRV_I2C_QUEUE_SIZE_IDX0];

/* I2C PLib Interface Initialization */
static const DRV_I2C_PLIB_INTERFACE drvI2C0PLibAPI = {

    /* I2C PLib Transfer Read Add function */
    .read_t = (DRV_I2C_PLIB_READ)I2C2_Read,

    /* I2C PLib Transfer Write Add function */
    .write_t = (DRV_I2C_PLIB_WRITE)I2C2_Write,


    /* I2C PLib Transfer Write Read Add function */
    .writeRead = (DRV_I2C_PLIB_WRITE_READ)I2C2_WriteRead,

    /*I2C PLib Transfer Abort function */
    .transferAbort = (DRV_I2C_PLIB_TRANSFER_ABORT)I2C2_TransferAbort,

    /* I2C PLib Transfer Status function */
    .errorGet = (DRV_I2C_PLIB_ERROR_GET)I2C2_ErrorGet,

    /* I2C PLib Transfer Setup function */
    .transferSetup = (DRV_I2C_PLIB_TRANSFER_SETUP)I2C2_TransferSetup,

    /* I2C PLib Callback Register */
    .callbackRegister = (DRV_I2C_PLIB_CALLBACK_REGISTER)I2C2_CallbackRegister,
};


static const DRV_I2C_INTERRUPT_SOURCES drvI2C0InterruptSources =
{
    /* Peripheral has more than one interrupt vector */
    .isSingleIntSrc                        = false,

    /* Peripheral interrupt lines */
    .intSources.multi.i2cInt0          = (int32_t)_I2C2_BUS_IRQ,
    .intSources.multi.i2cInt1          = (int32_t)_I2C2_MASTER_IRQ,
    .intSources.multi.i2cInt2          = -1,
    .intSources.multi.i2cInt3          = -1,
};

/* I2C Driver Initialization Data */
static const DRV_I2C_INIT drvI2C0InitData =
{
    /* I2C PLib API */
    .i2cPlib = &drvI2C0PLibAPI,

    /* I2C Number of clients */
    .numClients = DRV_I2C_CLIENTS_NUMBER_IDX0,

    /* I2C Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvI2C0ClientObjPool[0],

    /* I2C TWI Queue Size */
    .transferObjPoolSize = DRV_I2C_QUEUE_SIZE_IDX0,

    /* I2C Transfer Objects */
    .transferObjPool = (uintptr_t)&drvI2C0TransferObj[0],

    /* I2C interrupt sources */
    .interruptSources = &drvI2C0InterruptSources,

    /* I2C Clock Speed */
    .clockSpeed = DRV_I2C_CLOCK_SPEED_IDX0,
};
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="DRV_I2S Instance 0 Initialization Data">

/* I2S PLIB Interface Initialization */
DRV_I2S_PLIB_INTERFACE drvI2S0PlibAPI =
{
    .I2S_LRCLK_Get = (DRV_I2S_LRCLK_GET)I2S2_LRCLK_Get,  
    .I2S_RefClockSet = (DRV_I2S_REFCLOCKSET) I2S2_RefClockSet,
    .I2S_BaudRateSet = (DRV_I2S_BAUDRATESET) I2S2_BaudRateSet,
};

/* I2S Driver Initialization Data */
DRV_I2S_INIT drvI2S0InitData =
{
    /* I2S PLIB API */
    .i2sPlib = &drvI2S0PlibAPI,

    /* I2S IRQ */
    .interruptI2S = DRV_I2S_INT_SRC_IDX0,

    /* I2S Number of clients */
    .numClients = DRV_I2S_CLIENTS_NUMBER_IDX0,

    /* I2S Queue Size */
    .queueSize = DRV_I2S_QUEUE_SIZE_IDX0,  

    .dmaChannelTransmit = DRV_I2S_XMIT_DMA_CH_IDX0,
    .dmaChannelReceive  = DRV_I2S_RCV_DMA_CH_IDX0,
    .i2sTransmitAddress = (void *)&(SPI2BUF),
    .i2sReceiveAddress = (void *)&(SPI2BUF),

    /************ code specific to PIC32M. ********************/
    .interruptTxDMA = _DMA0_VECTOR,
    .interruptRxDMA = _DMA1_VECTOR,
    /************ end of PIC32M. specific code ********************/
    .dmaDataLength = DRV_I2S_DATA_LENGTH_IDX0,
};

// </editor-fold>

/*** CODEC Driver Initialization Data ***/
const DRV_WM8904_INIT drvwm8904Codec0InitData =
{
    .i2sDriverModuleIndex = DRV_WM8904_I2S_DRIVER_MODULE_INDEX_IDX0,
    .i2cDriverModuleIndex = DRV_WM8904_I2C_DRIVER_MODULE_INDEX_IDX0,
    .masterMode = DRV_WM8904_I2S_MASTER_MODE,
    .samplingRate = DRV_WM8904_AUDIO_SAMPLING_RATE,
    .volume = DRV_WM8904_VOLUME,
    .audioDataFormat = DRV_WM8904_AUDIO_DATA_FORMAT_MACRO,
    .enableMicInput = DRV_WM8904_ENABLE_MIC_INPUT,
    .enableMicBias = DRV_WM8904_ENABLE_MIC_BIAS,
    .micGain = DRV_WM8904_MIC_GAIN
};




// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************
/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************

static const SYS_DEBUG_INIT debugInit =
{
    .moduleInit = {0},
    .errorLevel = SYS_DEBUG_GLOBAL_ERROR_LEVEL,
    .consoleIndex = 0,
};


// <editor-fold defaultstate="collapsed" desc="SYS_TIME Initialization Data">

static const SYS_TIME_PLIB_INTERFACE sysTimePlibAPI = {
    .timerCallbackSet = (SYS_TIME_PLIB_CALLBACK_REGISTER)TMR1_CallbackRegister,
    .timerStart = (SYS_TIME_PLIB_START)TMR1_Start,
    .timerStop = (SYS_TIME_PLIB_STOP)TMR1_Stop ,
    .timerFrequencyGet = (SYS_TIME_PLIB_FREQUENCY_GET)TMR1_FrequencyGet,
    .timerPeriodSet = (SYS_TIME_PLIB_PERIOD_SET)TMR1_PeriodSet,
};

static const SYS_TIME_INIT sysTimeInitData =
{
    .timePlib = &sysTimePlibAPI,
    .hwTimerIntNum = 4,
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_CONSOLE Instance 0 Initialization Data">


static const SYS_CONSOLE_UART_PLIB_INTERFACE sysConsole0UARTPlibAPI =
{
    .read_t = (SYS_CONSOLE_UART_PLIB_READ)UART1_Read,
    .readCountGet = (SYS_CONSOLE_UART_PLIB_READ_COUNT_GET)UART1_ReadCountGet,
    .readFreeBufferCountGet = (SYS_CONSOLE_UART_PLIB_READ_FREE_BUFFFER_COUNT_GET)UART1_ReadFreeBufferCountGet,
    .write_t = (SYS_CONSOLE_UART_PLIB_WRITE)UART1_Write,
    .writeCountGet = (SYS_CONSOLE_UART_PLIB_WRITE_COUNT_GET)UART1_WriteCountGet,
    .writeFreeBufferCountGet = (SYS_CONSOLE_UART_PLIB_WRITE_FREE_BUFFER_COUNT_GET)UART1_WriteFreeBufferCountGet,
};

static const SYS_CONSOLE_UART_INIT_DATA sysConsole0UARTInitData =
{
    .uartPLIB = &sysConsole0UARTPlibAPI,
};

static const SYS_CONSOLE_INIT sysConsole0Init =
{
    .deviceInitData = (const void*)&sysConsole0UARTInitData,
    .consDevDesc = &sysConsoleUARTDevDesc,
    .deviceIndex = 0,
};



// </editor-fold>




// *****************************************************************************
// *****************************************************************************
// Section: Local initialization functions
// *****************************************************************************
// *****************************************************************************

/* MISRAC 2012 deviation block end */

/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{

    /* MISRAC 2012 deviation block start */
    /* MISRA C-2012 Rule 2.2 deviated in this file.  Deviation record ID -  H3_MISRAC_2012_R_2_2_DR_1 */

    /* Start out with interrupts disabled before configuring any modules */
    (void)__builtin_disable_interrupts();

  
    CLK_Initialize();

    /* Configure KSEG0 as cacheable memory. This is needed for Prefetch Buffer */
    __builtin_mtc0(16, 0,(__builtin_mfc0(16, 0) | 0x3U));

    /* Configure Flash Wait States and Prefetch */
    CHECONbits.PFMWS = 1;
    CHECONbits.PREFEN = 3;

    /* Set the SRAM wait states to One */
    BMXCONbits.BMXWSDRM = 1;




	GPIO_Initialize();

    DMAC_Initialize();
    I2C2_Initialize();
	BSP_Initialize();
	UART1_Initialize();

    TMR1_Initialize();

	I2S2_Initialize();



    /* MISRAC 2012 deviation block start */
    /* Following MISRA-C rules deviated in this block  */
    /* MISRA C-2012 Rule 11.3 - Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
    /* MISRA C-2012 Rule 11.8 - Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */

    /* Initialize I2C0 Driver Instance */
    sysObj.drvI2C0 = DRV_I2C_Initialize(DRV_I2C_INDEX_0, (SYS_MODULE_INIT *)&drvI2C0InitData);

    /* Initialize I2S0 Driver Instance */
    sysObj.drvI2S0 = DRV_I2S_Initialize(DRV_I2S_INDEX_0, (SYS_MODULE_INIT *)&drvI2S0InitData);

    sysObj.drvwm8904Codec0 = DRV_WM8904_Initialize(DRV_WM8904_INDEX_0, (SYS_MODULE_INIT *)&drvwm8904Codec0InitData);


    /* MISRA C-2012 Rule 11.3, 11.8 deviated below. Deviation record ID -  
     H3_MISRAC_2012_R_11_3_DR_1 & H3_MISRAC_2012_R_11_8_DR_1*/
        
    sysObj.sysDebug = SYS_DEBUG_Initialize(SYS_DEBUG_INDEX_0, (SYS_MODULE_INIT*)&debugInit);

    /* MISRAC 2012 deviation block end */
    /* MISRA C-2012 Rule 11.3, 11.8 deviated below. Deviation record ID -  
    H3_MISRAC_2012_R_11_3_DR_1 & H3_MISRAC_2012_R_11_8_DR_1*/
        
    sysObj.sysTime = SYS_TIME_Initialize(SYS_TIME_INDEX_0, (SYS_MODULE_INIT *)&sysTimeInitData);
    
    /* MISRAC 2012 deviation block end */
    /* MISRA C-2012 Rule 11.3, 11.8 deviated below. Deviation record ID -  
     H3_MISRAC_2012_R_11_3_DR_1 & H3_MISRAC_2012_R_11_8_DR_1*/
        sysObj.sysConsole0 = SYS_CONSOLE_Initialize(SYS_CONSOLE_INDEX_0, (SYS_MODULE_INIT *)&sysConsole0Init);
   /* MISRAC 2012 deviation block end */


    /* MISRAC 2012 deviation block end */
    APP_Initialize();
    APP_S1LED_Initialize();


    EVIC_Initialize();

	/* Enable global interrupts */
    (void)__builtin_enable_interrupts();



    /* MISRAC 2012 deviation block end */
}

/*******************************************************************************
 End of File
*/
