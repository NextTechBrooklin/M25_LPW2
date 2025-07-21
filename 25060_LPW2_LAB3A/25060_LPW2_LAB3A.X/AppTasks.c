#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include <string.h>
#include "definitions.h"                // SYS function prototypes
#include "../25060_LPW2_LAB3A.X/myDefinitions.h"
#include "../25060_LPW2_LAB3A.X/AppTasks.h"
#include "../25060_LPW2_LAB3A.X/LEDTasks.h"
#include "../25060_LPW2_LAB3A.X/main.h"


extern APP_STATES myApp;
extern APP_STRUCT myAppData;

void AppTask(void){
    switch(myApp){
        case INIT:
            TC4_TimerStart(); 
            ADC_ConversionStart();            
            myApp = SAMPLE;
            break;
        case SAMPLE:
            if(myAppData.tick == 1){
                myAppData.tick = 0;
                if(ADC_ConversionStatusGet()!=0 ){
                    myAppData.sample = ADC_ConversionResultGet(); 
                    ADC_ConversionStart();
                    myApp = TRANSMIT;
                }
            }
            break;
        case TRANSMIT:
            sprintf((char*)myAppData.TXData,"< %d >",myAppData.sample);
            //Hint:  modify code above to add output of switch state
            SERCOM3_USART_Write(myAppData.TXData,strlen((char *)myAppData.TXData));
            myApp = SAMPLE;
            break;            
    }
    
}

void sampleTimer(TC_TIMER_STATUS event,uintptr_t context ){
    myAppData.tick = 1;
}