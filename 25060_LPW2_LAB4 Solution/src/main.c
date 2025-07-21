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
#include <stdio.h>
#include <string.h>
#include "definitions.h"                // SYS function prototypes
#include "../25060_LPW2_LAB4.X/myDefinitions.h"
#include "../25060_LPW2_LAB4.X/AppTasks.h"
#include "../25060_LPW2_LAB4.X/LEDTasks.h"
#include "../25060_LPW2_LAB4.X/main.h"


uintptr_t context1 = 0;
uintptr_t context2 = 0;
uintptr_t context3 = 0;
TC_TIMER_STATUS status;

RN_STATES RNState = RN_INIT;
APP_STATES myApp = SAMPLE;
APP_STRUCT myAppData;

extern LED_BLINKS LedBlinkPattern;
ADC_VALUE adcValue;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    InitSystem();
    myApp = INIT;    
    while ( true )
    {
        LEDTask();

        switch(RNState){
            case RN_INIT:
                LedBlinkPattern = SLOW_BLINK;
                sprintf((char*)myAppData.TXData,"$$$");
                SERCOM3_USART_Write(myAppData.TXData,strlen((char *)myAppData.TXData));                 
                SYSTICK_DelayMs(300);    
                TC4_TimerStart();                  
                RNState = BEACON;
                break;
            case BEACON:
                LedBlinkPattern = SHORT_BLINK;
                AppTask();
                break; 
            default:
                break;
        }
    }
    /* Execution should not come here during normal operation */
    return ( EXIT_FAILURE );
}




void InitSystem(void){
    // register all call backs
    SERCOM3_USART_ReadNotificationEnable(1,1);
    SERCOM3_USART_ReadThresholdSet(1);
    SERCOM3_USART_WriteNotificationEnable(1,1);
    SERCOM3_USART_WriteThresholdSet(1);
    SERCOM5_USART_ReadNotificationEnable(1,1);
    SERCOM5_USART_ReadThresholdSet(1);
    SERCOM3_USART_ReadCallbackRegister( mySercom3, context1 );
    SERCOM5_USART_ReadCallbackRegister( mySercom5, context1 );
    TC3_TimerCallbackRegister( blinkTimer, context2 );
    TC4_TimerCallbackRegister( sampleTimer, context3 );

    ADC_Enable();
    SYSTICK_TimerStart();
    TC3_TimerStart();     

    myAppData.tick = 0;
    myAppData.LEDTick = 0;
    InitRN();

    
}


void InitRN(void){
    RN_MODE_Clear();// set into command mode  ****  current errata this does not work    
    RN_RESET_Clear();
    SYSTICK_DelayMs(10);
    RN_RESET_Set();
    SYSTICK_DelayMs(200);
    if(SW0_Get() == 0){
        myAppData.passThrough = 1;
        LedBlinkPattern = FAST_BLINK;
        while(1){
            LEDTask();
        }
    }
    else
        myAppData.passThrough = 0;  
    SYSTICK_DelayMs(300);    
}


void mySercom3(SERCOM_USART_EVENT event, uintptr_t context ){
uint8_t buf[10];
uint8_t cnt;    
    if(myAppData.passThrough == 1){
        cnt = SERCOM3_USART_ReadCountGet();
        if(cnt>0){
            SERCOM3_USART_Read(buf,cnt);
            SERCOM5_USART_Write(buf,cnt);
        }        
    }
    else{
        
    }
}

void mySercom5(SERCOM_USART_EVENT event, uintptr_t context ){
uint8_t buf[10];
uint8_t cnt;
    if(myAppData.passThrough == 1){
        cnt = SERCOM5_USART_ReadCountGet();
        if(cnt>0){
            SERCOM5_USART_Read(buf,cnt);
            SERCOM3_USART_Write(buf,cnt);
        }
    }
    else{
        
    }
}






/*******************************************************************************
 End of File
*/

