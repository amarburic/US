#include "mbed.h"

#define _A 10
#define _B 11
#define _C 12
#define _D 13
#define _zvijezda 14
#define _taraba 15

#define dp23 P0_0

BusOut ledovi(dp23, dp24, dp25, dp26, dp27, dp5, dp6, dp28);
BusOut redovi(dp16, dp15, dp17, dp18);
BusIn kolone(dp9, dp10, dp11, dp13);

DigitalOut enable(dp14);

const int tastatura[4][4] = 
{ 
    {1, 2, 3, _A}, 
    {4, 5, 6, _B}, 
    {7, 8, 9, _C},
    {_zvijezda, 0, _taraba, _D}
};

int ocitaj() {
    for(int i = 0; i <= 3; i++) {
        redovi = 1 << i;  
            for(int j = 0; j <= 3; j++)
                if(kolone & (1 << j))
                    return tastatura[i][j];
    }
    return -1;
}

int main() {
    enable = 0;
    ledovi = 255;
    while(1) {
        char ucitano(ocitaj());
        char maska = 0;
        if(ucitano >= 1 && ucitano < 9) {
            maska = 1 << (ucitano - 1);
        }
        else if(ucitano == _zvijezda) //Dodatni zadatak, ugasi svaku drugu 
            maska = 170; 
        maska = maska ^ 255;
        ledovi = maska;
    }
}
