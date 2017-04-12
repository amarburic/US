#include "mbed.h"

#define T 0.00005

PwmOut led1(dp18);
AnalogIn potenciometar(dp9);
DigitalOut enable(dp14);

int main() {
    enable = 1;
    
    led1.period(T);
    
    float zadnja_vrijednost(potenciometar.read());
    
    led1.write(zadnja_vrijednost);
    
    while(1) {
        float nova_vrijednost = potenciometar.read();
        if(zadnja_vrijednost != nova_vrijednost) {
            zadnja_vrijednost = nova_vrijednost;
            led1.write(nova_vrijednost);
            if(nova_vrijednost < 0.1)
                led1.write(0);
            
        }
    }
}
