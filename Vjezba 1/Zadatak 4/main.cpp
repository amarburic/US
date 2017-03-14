#include "mbed.h"


#define dp23 P0_0

DigitalOut leds[8] = {dp28, dp6, dp5, dp27, dp26, dp25, dp24, dp23}; 
DigitalOut enable(dp14);

int main() {
    enable = 0;
    for(int i = 0; i <= 7 ; i++) {
        leds[i] = 0;
    }
    int index = 0; 
    double delay = 0.2;
    while(1) {
        wait(delay);
        leds[(index + 1) % 8] = 1;
        leds[index] = 0;
        index = (index + 1) % 8;
    }
    
}
