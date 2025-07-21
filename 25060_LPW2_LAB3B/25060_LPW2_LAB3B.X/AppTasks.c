#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include <string.h>
#include "definitions.h"                // SYS function prototypes
#include "../25060_LPW2_LAB3B.X/myDefinitions.h"
#include "../25060_LPW2_LAB3B.X/AppTasks.h"
#include "../25060_LPW2_LAB3B.X/LEDTasks.h"
#include "../25060_LPW2_LAB3B.X/main.h"


extern APP_STATES myApp;
extern APP_STRUCT myAppData;

void AppTask(void){
static uint8_t status;    
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
            myApp = READ;
            break;  
        case READ:
            status = AppReadByte();
            if(status == 1)
                myApp = DECODE;
            else
                myApp = SAMPLE;
            break;        
        case DECODE:
            sscanf(myAppData.RXData,"%s %d", myAppData.setCMD,&myAppData.setPoint);
            myApp = SAMPLE;
            break;        
    }
    // Hint add code here to test and set led on or off


    
}

uint8_t AppReadByte(void){
uint8_t buffer[5];   
uint8_t status = 0;
uint8_t count = 0;
    count = SERCOM3_USART_ReadCountGet();    
    if(count>0){
        SERCOM3_USART_Read(buffer,1);
        if(buffer[0]=='<')
            myAppData.RXLength = 0;
        else if(buffer[0] == '>'){
            myAppData.RXData[myAppData.RXLength++] = 0; // null pad  
            status = 1;        
        }
        else
            myAppData.RXData[myAppData.RXLength++] = buffer[0];       
    }
    return(status);
}


void sampleTimer(TC_TIMER_STATUS event,uintptr_t context ){
    myAppData.tick = 1;
}