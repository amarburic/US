#include "mbed.h"
#define dp23 P0_0

Serial pc(USBTX,USBRX);

// Analogni ulazi sa dzojstika
AnalogIn VRx ( dp11 );
AnalogIn VRy ( dp10 );

// Taster sa dzojstika
DigitalIn SW ( dp9 );

// Digitalni izlaz za aktivaciju LED buffera
DigitalOut enable ( dp14 );

// LED prikaz
BusOut prikazX ( dp28 , dp6 , dp5 );
BusOut prikazY ( dp27 , dp26 , dp25 );
DigitalOut prikazSW ( dp23 );

enum Polozaj {centar, lijevo, desno, gore, dole};

Polozaj polozaj = centar;
bool prosliSW = false;
bool ispisi = true;


void ispis() {
    switch(polozaj) {
        case centar: 
            pc.puts("centar");
        break;
        case lijevo: 
            pc.puts("lijevo");
        break;
        case desno: 
            pc.puts("desno");
        break;
        case gore: 
            pc.puts("gore");
        break;
        case dole: 
            pc.puts("dole");
        break;
    }
    pc.puts("\r\n");
}

int main () {
    // aktivacija LED dioda
    enable =1;
    // Aktivacija pull - up otpornika
    SW.mode( PullUp );
    while (1) {
        Polozaj noviPolozaj;
        
    
        if (VRx <1.0/3.0) noviPolozaj =lijevo;
        else if(VRx > 2.0/3.0) noviPolozaj = desno;
        else if ( VRy <1.0/3.0) noviPolozaj = dole;
        else if ( VRy > 2.0/3.0) noviPolozaj = gore;
        else noviPolozaj =centar;
        
        if(SW == 1) {
            if(!prosliSW)
            pc.puts("klik\n");
            prosliSW = 1;
            
        } else prosliSW = 0;
        if(noviPolozaj != polozaj) {
            polozaj = noviPolozaj;
            ispis(); 
        }        
        wait (0.001);
    }
}