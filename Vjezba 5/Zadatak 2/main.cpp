
#include "mbed.h"
#define dp23 P0_0
#define DEBOUNCING

BusOut prikaz1 ( dp26 , dp25 , dp24 , dp23 );
BusOut prikaz2 ( dp28 , dp6 , dp5 , dp27 );

DigitalOut enable ( dp14 );
//DigitalIn taster ( dp1 );
InterruptIn taster(dp1);
#ifdef DEBOUNCING
Timer debounce;
#endif

Ticker incrementer1, incrementer2;

const float T (2);
#ifdef DEBOUNCING
const float DT(0.2);
#endif
int brojac1 (0);
int brojac2 (0);
bool taster_ukljucen(false);

void inc1() {
    brojac1 =( brojac1 +1)%16;
    prikaz1 = brojac1;
}

void inc2() {
     if(debounce.read() > DT) {
        brojac2 =( brojac2 +1)%16;
        prikaz2 = brojac2;
        debounce.reset();
    }
}

void ukljucivanje_tastera() {
    #ifdef DEBOUNCING
    if(debounce.read() > DT) {
        #endif
        taster_ukljucen = true;
        #ifdef DEBOUNCING
        debounce.reset();
    }
    #endif
}

void iskljucivanje_tastera() {
    #ifdef DEBOUNCING
    if(debounce.read() > DT) {
        #endif
        taster_ukljucen = false;
        #ifdef DEBOUNCING
        debounce.reset();
    }
    #endif
}

int main () {
    enable = 0;
    prikaz1 = brojac1 ;
    prikaz2 = brojac2 ;
    
    incrementer1.attach(&inc1, T);
    //incrementer2.attach(&inc2, T);
    
    #ifdef DEBOUNCING
    debounce.start();
    #endif
    
    taster.rise(&inc2);
  //  taster.fall(&iskljucivanje_tastera);
        
    while(1);
    
    
    
}

