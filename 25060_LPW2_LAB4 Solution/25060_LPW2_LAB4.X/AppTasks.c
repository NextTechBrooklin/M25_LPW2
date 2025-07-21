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

extern APP_STATES myApp;
extern APP_STRUCT myAppData;
extern ADC_VALUE adcValue;

uint8_t switchStatus;

void AppTask(void){
    if(myAppData.tick == 1){
        myAppData.tick = 0;
        switch(myApp){
            case INIT:
                ADC_ConversionStart();            
                myApp = SAMPLE;
                break;
            case SAMPLE:
                if(ADC_ConversionStatusGet()!=0 ){
                    myAppData.sample = ADC_ConversionResultGet(); 
                    ADC_ConversionStart();
                    myApp = CLEAR_BUFFER;
                }
                break;
            case CLEAR_BUFFER:
                sprintf((char*)myAppData.TXData,"IB,Z\n\r");
                SERCOM3_USART_Write(myAppData.TXData,strlen((char *)myAppData.TXData));            
                while (SERCOM3_USART_TransmitComplete()!=true){}
                myApp = LOAD_BUFFER;
                break;
            case LOAD_BUFFER:
                switchStatus = SW0_Get();
                adcValue.value = myAppData.sample;
                sprintf((char*)myAppData.TXData,"IB,FF,F000%02X%02X%02X\n\r",
                        adcValue.bytes[1],adcValue.bytes[0],switchStatus);
                SERCOM3_USART_Write(myAppData.TXData,strlen((char *)myAppData.TXData));
                while (SERCOM3_USART_TransmitComplete()!=true){}
                myApp = SAMPLE;            
                break;            
        }
    }
}

void sampleTimer(TC_TIMER_STATUS event,uintptr_t context ){
    myAppData.tick = 1;
}

