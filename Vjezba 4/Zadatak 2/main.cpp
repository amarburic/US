#include "mbed.h"
#define T 0.5
#define dp23 P0_0

BusOut ledovi(dp23, dp24, dp25, dp26, dp27, dp5, dp6, dp28);
AnalogIn otpornik(dp9);
DigitalOut enable(dp14);

int main() {
    while(1) {
        ledovi = (1 << ((int)(otpornik * 8))) - 1;
    }
}
