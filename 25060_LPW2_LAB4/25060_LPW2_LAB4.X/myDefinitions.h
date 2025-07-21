
typedef union U1 {uint16_t value;
          uint8_t bytes[2];}ADC_VALUE;
          

typedef enum{   RN_INIT = 0x00,
                BEACON = 0x01
                    }RN_STATES;
                
typedef enum {INIT,SAMPLE,CLEAR_BUFFER,LOAD_BUFFER}APP_STATES;

typedef enum{   SLOW_BLINK,
                FAST_BLINK,
                SHORT_BLINK,}LED_BLINKS;


typedef struct {uint8_t tick;
                uint8_t LEDTick;
                uint8_t passThrough;
                uint16_t sample;
                uint8_t TXData[128];
                uint8_t TXLength;
                }APP_STRUCT;
                
                





                
                


