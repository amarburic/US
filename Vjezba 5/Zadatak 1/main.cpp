#include "mbed.h"
#define dp23 P0_0

DigitalOut enable(dp14);

BusOut displej(dp26, dp27, dp5, dp6, dp28, dp4, dp18, dp17);
BusOut odabir_cifre(dp25, dp24, dp23);

InterruptIn taster1(dp1);
InterruptIn taster2(dp2);

const int lookup_tabela[] = {
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
 
//tabela sa tackama
const int lookup_tabela_tacka[] = {
    0x40 ,
    0x79 ,
    0x24 ,
    0x30 ,
    0x19 ,
    0x12 ,
    0x03 ,
    0x78 ,
    0x00 ,
    0x10
};

int pozicija_cifre_za_ispis(1);
int brojac(999);

const float T(0.1);
const float DT(0.2);
const float TI(0.001);

bool broji(false);

Ticker t_ispis, t_brojac;
Timer debounce_t1, debounce_t2;

int nta_cifra(int broj, int n) {
    for(int i = 1; i < n; i++)
        broj /= 10;
    return broj % 10;
}

void ispisi_cifru(int cifra, int pozicija) {
    if(pozicija == 4)
        return;
    odabir_cifre = 7 - (1 << (pozicija - 1));
    if(pozicija != 2)
        displej = lookup_tabela[cifra];
    else
        displej = lookup_tabela_tacka[cifra];//druga cifra treba sadrzavati tacku
}

void ispisi_na_displej() {
    ispisi_cifru(nta_cifra(brojac, pozicija_cifre_za_ispis), 
    pozicija_cifre_za_ispis);
    pozicija_cifre_za_ispis = (pozicija_cifre_za_ispis + 1) % 4;
    if(pozicija_cifre_za_ispis == 0)
        pozicija_cifre_za_ispis++;
}

void start_stop() {
    if(debounce_t1.read() > DT) {
        broji = 1 - broji;
        debounce_t1.reset(); 
    }
}

void reset() {
    if(debounce_t2.read() > DT) {
        broji = false;
        brojac = 999;
        debounce_t2.reset(); 
    }
}

void ink() {
    if(broji)
        brojac--;
    if(brojac == -1)
        brojac = 999;
}

int main() {
    
    enable = 1;
    
    t_ispis.attach(&ispisi_na_displej, TI); //Ispisuje cifru na displej
    t_brojac.attach(&ink, T); //Inkrementuje brojac
    taster1.rise(&start_stop); //Vezivanje tastera 1 za start stop funkciju
    taster2.rise(&reset); //Vezivanje tastera 2 za reset funkciju
    debounce_t1.start(); //Debounce tastera 1
    debounce_t2.start(); //Debounce tastera 2
    while(1);
}
