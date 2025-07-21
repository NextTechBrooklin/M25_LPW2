

typedef union
{   struct { 
        int CONNECT:1;
        int STREAM:1;
        int NA:6;
        }u;
    uint8_t state;
}RN_PINS;

typedef enum{   DISC = 0x00,
                CONN = 0x01,
                STREAM = 0x03,}RN_APP_STATES;
                
typedef enum {INIT,SAMPLE,TRANSMIT,READ,DECODE}APP_STATES;

typedef enum{   SLOW_BLINK,
                FAST_BLINK,
                SHORT_BLINK,}LED_BLINKS;


typedef struct {uint8_t tick;
                uint8_t LEDTick;
                uint8_t passThrough;
                uint16_t sample;
                int setPoint;
                uint8_t TXData[128];
                uint8_t TXLength;
                char RXData[128];
                uint8_t RXLength;
                char setCMD[16];
                }APP_STRUCT;
                
                





                
                


