#include "mbed.h"

DigitalOut red(LED1);
DigitalOut green(LED2);
DigitalOut blue(LED3);

int main() {
    int delay = 10;
    red = green = blue = 1;
    while(1) {
        red = 0;
        wait(delay / 10.0);
        red = 1;
        green = 0;
        wait(delay / 10.0);
        green = 1;
        blue = 0;
        wait(delay / 10.0);
        blue = 1;
        if(delay > 1)
            delay--;
    }
}
