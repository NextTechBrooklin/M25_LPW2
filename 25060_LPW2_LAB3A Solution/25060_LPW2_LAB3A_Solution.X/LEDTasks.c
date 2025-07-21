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


extern APP_STRUCT myAppData;
LED_BLINKS LedBlinkPattern = SLOW_BLINK;


void LEDTask(void){
static uint8_t count = 0;   
static uint8_t doubleCount = 0;
    if(myAppData.LEDTick == 1){
        myAppData.LEDTick = 0;
        switch(LedBlinkPattern){
            case SLOW_BLINK:
                count+=1;
                if(count>10){
                    count = 0;
                    LED_Toggle();
                }
                break;
            case FAST_BLINK:
                LED_Toggle();
                break;
            case SHORT_BLINK:
                doubleCount +=1;
                LED_Set();
                if(doubleCount == 7){
                    doubleCount = 0;
                    LED_Clear();
                }
                break;  
            default:
                break;
        }
    }
}

void blinkTimer(TC_TIMER_STATUS event,uintptr_t context ){
    myAppData.LEDTick = 1;
}