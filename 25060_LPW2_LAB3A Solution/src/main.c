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
#include "../25060_LPW2_LAB3A_Solution.X/myDefinitions.h"
#include "../25060_LPW2_LAB3A_Solution.X/AppTasks.h"
#include "../25060_LPW2_LAB3A_Solution.X/LEDTasks.h"
#include "../25060_LPW2_LAB3A_Solution.X/main.h"


uintptr_t context1 = 0;
uintptr_t context2 = 0;
uintptr_t context3 = 0;
TC_TIMER_STATUS status;

volatile APP_STRUCT myAppData;
APP_STATES myApp = SAMPLE;
RN_PINS RNPins;
RN_APP_STATES RNAppState;
APP_STATES AppState = SAMPLE;

extern LED_BLINKS LedBlinkPattern;

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
    
    while ( true )
    {
        LEDTask();
        RNPins.u.CONNECT = RN_STAT_IND1_Get();
        RNPins.u.STREAM = RN_STAT_IND2_Get();
        RNAppState = RNPins.state;    

        switch(RNAppState){
            case DISC:
                LedBlinkPattern = SLOW_BLINK;
                TC4_TimerStop();
                myApp = INIT;
                break;
            case CONN:
                                
                break;
            case STREAM:
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

    RNPins.u.CONNECT = 0;
    RNPins.u.STREAM = 0;
    myAppData.tick = 0;
    myAppData.LEDTick = 0;
    myAppData.setPoint = 1024;
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

