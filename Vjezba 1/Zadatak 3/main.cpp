#include "mbed.h"

#define dp23 P0_0

DigitalOut leds[8] = {dp28, dp6, dp5, dp27, dp26, dp25, dp24, dp23};
DigitalIn taster(dp1);
DigitalOut enable(dp14);

int main() {
    int delay = 1;
    enable = 0;
    for(int i = 0; i < 8 ; i++) {
        leds[i] = 1;
    }    
    while(1) {
        wait(delay);
        if(taster) //oduzmi
            for(int i = 7; i >= 0; i--) {
                leds[i] = 1 - leds[i];
                if(!leds[i]) 
                    break;
            }
        else 
            for(int i = 7; i >= 0; i--) {
                leds[i] = 1 - leds[i];
                if(leds[i]) 
                    break;
            }
    }
}
