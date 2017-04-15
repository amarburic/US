#include "mbed.h"
#define dp23 P0_0

BusOut redovi(dp2, dp1, dp28, dp6);
BusIn kolone(dp9, dp10, dp11, dp13);
Serial pc(USBTX,USBRX);

DigitalOut enable(dp14);
Ticker citac;
bool ispisi = true;
char ucitano = 0;

const char tastatura[4][4] = 
{ 
    {'1', '2', '3', 'A'}, 
    {'4', '5', '6', 'B'}, 
    {'7', '8', '9', 'C'},
    {'\n', '0', '#', 'D'}
};

char ocitaj() {
    for(int i = 0; i <= 3; i++) {
        redovi = 1 << i;  
            for(int j = 0; j <= 3; j++)
                if(kolone & (1 << j))
                    return tastatura[i][j];
    }
    return 0;
}


void citaj() {
    ucitano = ocitaj();
    if(ucitano == 0) {
        ispisi = true;
        return;
    }        
}

int main() {
    enable = 1;
    citac.attach(&citaj, 0.001);
    while(1) {
        if(ucitano != 0 && ispisi) {
            pc.putc(ucitano);
            if(ucitano == '\n')
                pc.putc('\r');
            ispisi = false;
        }
    }
}
