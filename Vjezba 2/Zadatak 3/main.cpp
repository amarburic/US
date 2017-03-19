#include "mbed.h"
#include <vector>
#include <ctime>
#include <cmath>

#define _A 10
#define _B 11
#define _C 12
#define _D 13
#define _zvijezda 14
#define _taraba 15
#define BROJ_CIKLUSA 75
#define PRAZNO 10

#define dp23 P0_0

BusOut displej(dp26, dp27, dp5, dp6, dp28, dp1, dp2, dp4);
BusOut odabir_cifre(dp25, dp24, dp23);
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
    0x10 ^ (1 << 7), 
    0xFF
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
    int c = nta_cifra(broj, pozicija);
    if(!(pow(10.0, pozicija - 1.0) > broj && c == 0 && pozicija != 1))
        ispisi_cifru(c, pozicija);
    else 
        ispisi_cifru(PRAZNO, pozicija); 
    pozicija = (pozicija + 1) % 4;
    if(pozicija == 0) 
        pozicija++;
}

int ocitaj(vector<vector<bool> > &ugasen) {
    int ocitani_znak = -1;
    for(int i = 0; i <= 3; i++) {
        redovi = 1 << i;
        for(int j = 0; j <= 3; j++) {
            if(kolone & 1 << j) {
                if(ugasen[i][j]) {
                    ugasen[i][j] = false;
                    ocitani_znak = tastatura[i][j];
                }
            } else
                ugasen[i][j] = true;
        }
    }
    return ocitani_znak;
}

void procesiraj_unos(int &vrijednost, int unos) {
    if(unos == _C) {
        vrijednost = 0;
        return;
    }
    if(vrijednost >= 100 && vrijednost <= 999)
        return;
    if(unos >= 0 && unos <= 9)
        vrijednost = (vrijednost * 10 + unos);
}

bool proteklo_vrijeme(clock_t &zadnji_refresh, float interval) {
    clock_t trenutno_vrijeme = clock();
    if((trenutno_vrijeme - zadnji_refresh) / (CLOCKS_PER_SEC / 1000.0) >= interval) {
        zadnji_refresh = trenutno_vrijeme;
        return true;
    }
    return false;
} 

int main() {
    vector<vector<bool> > ugasen(4, vector<bool>(4, true));
    int vrijednost(0);
    int cifra_za_ispis(1);
    float interval_refresha(1.0 / BROJ_CIKLUSA / 3); 
    clock_t zadnji_refresh(clock());
    while(1) {
        wait(interval_refresha);
        ispisi(vrijednost, cifra_za_ispis);
        procesiraj_unos(vrijednost, ocitaj(ugasen));
    }
}
