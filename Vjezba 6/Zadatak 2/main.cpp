#include "mbed.h"
#define dp23 P0_0

BusOut ledovi(dp23, dp24, dp25, dp26, dp27, dp5, dp6, dp28);
Serial pc(USBTX,USBRX);
DigitalOut enable ( dp14 );

int ucitano = 0;
int broj = 0;

int dajBroj() {
    return pc.getc() - '0';
}

int dajTrocifrenBroj() {
    return dajBroj() * 100 + dajBroj() * 10 + dajBroj();
}

int main() {
    enable = 0;
    ledovi = 0;
    while(1) {
            if(pc.readable()) {
                 if(ucitano < 3) {
                    broj *= 10;
                    broj += dajBroj();
                    ucitano++;
                }
                if(ucitano == 3) {
                    ledovi = broj;
                    ucitano = 0;
                    broj = 0;
                }
            }
        wait(0.001);
        
    }
    
}
