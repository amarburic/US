#include "mbed.h"
#define T_f 0.0025
#define N_step 50
#define T_step (T_f / N_step)
#define Umax 1


#include <cmath>
#define Pi 3.14
AnalogOut osciloskop(PTE30);


float funkcija(float t) {
    if(t >= (T_f / 2))
        t -=(T_f / 2);
        
    return t / (T_f / 2);
}


int main() {
    float vrijednosti[N_step + 1];
    
    for(int i = 0; i <= N_step; i++)
        vrijednosti[i] = funkcija(i * T_step);
    int counter = 0;
    while(1) {
        osciloskop = vrijednosti[counter++];
        if(counter == N_step + 1)
            counter = 0;
        wait(T_step);    
    }
}
