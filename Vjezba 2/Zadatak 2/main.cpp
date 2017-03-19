#include "mbed.h"
#include <ctime>

#define BROJ_CIKLUSA 75
#define dp23 P0_0

DigitalIn taster1(dp1);
DigitalIn taster2(dp2);

BusOut displej(dp26, dp27, dp5, dp6, dp28, dp4, dp18, dp17);
BusOut odabir_cifre(dp25, dp24, dp23);

DigitalOut enable(dp14);

//Lookup table za common anode
//Xoranje sa 1 << 7 je uradjeno da se ugasi tacka
int lookup_tabela[] = { 
    0x40 ^ (1 << 7), 
    0x79 ^ (1 << 7), 
    0x24 ^ (1 << 7),
    0x30 ^ (1 << 7), 
    0x19 ^ (1 << 7), 
    0x12 ^ (1 << 7), 
    0x03 ^ (1 << 7), 
    0x78 ^ (1 << 7), 
    0x00 ^ (1 << 7), 
    0x10 ^ (1 << 7)
};

int nta_cifra(int broj, int n) {
    for(int i = 1; i < n; i++)
        broj /= 10;
    return broj % 10;
}

void ispisi_cifru(int cifra, int pozicija) {
    odabir_cifre = 1 << (pozicija - 1);
    odabir_cifre = 7 - odabir_cifre;
    displej = lookup_tabela[cifra];    
}

void ispisi(int broj, int &pozicija) {
    ispisi_cifru(nta_cifra(broj, pozicija), pozicija); 
    pozicija = (pozicija + 1) % 4;
    if(pozicija == 0) 
        pozicija++;
}

void ink(int &brojac) {
    brojac = (brojac + 1) % 1000;
}

void dek(int &brojac) {
    brojac = (brojac - 1);
    if(brojac < 0)
        brojac = 999;
}

bool ocitaj_taster(DigitalIn &taster, bool &ugasen) {
    if(ugasen && taster) {
        ugasen = false;
        return true;
    } else if(!taster)
        ugasen = true; 
    return false;
}

int main() {
    enable = 1;
    bool ugasen1(true), ugasen2(true);
    int brojac(0), cifra_za_ispis(1);
    float interval_refresha(1. / BROJ_CIKLUSA / 3);
    while(1) {
        wait(interval_refresha);
        ispisi(brojac, cifra_za_ispis);
        if(ocitaj_taster(taster1, ugasen1))
            ink(brojac);
        if(ocitaj_taster(taster2, ugasen2))
            dek(brojac); 
    }
    
}
