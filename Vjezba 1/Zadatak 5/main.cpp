#include "mbed.h"

DigitalOut myled(LED1);

int main() {
    double T = 1;
    double t = 0;
    double uklj = T , isklj = T;
    while(1) {
        while(t < 30 * T) {
            myled = 1;
            wait(uklj);
            myled = 0;
            wait(isklj);
            t += 2 * T;
            uklj += 0.9 / 30 * 2 * T;
            isklj = 2 * T - uklj; 
        }
        while(t < 60 * T) {
            myled = 1;
            wait(uklj);
            myled = 0;
            wait(isklj);
            t += 2 * T; 
            uklj -= 1.8 / 30  * 2 * T;
            isklj = 2 * T - uklj;
        }
        t = -30 * T;
    }
}
