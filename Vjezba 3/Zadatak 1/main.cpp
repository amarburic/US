#include "mbed.h"
#include "N5110.h"
#include <string>
#define dp23 P0_0

const float VCC = 3.3;
const float R = 100000;
 
//N5110 lcd(VCC,SCE,RST,DC,MOSI,SCLK,LED));
N5110 lcd(dp4,dp24,dp23,dp25,dp2,dp6,dp18);

AnalogIn voltage(dp9);

DigitalOut enable(dp14);
DigitalIn taster(dp1);

bool ocitaj_taster(bool &ugasen) {
    if(ugasen && taster) {
        ugasen = false;
        return true;
    } else if(!taster)
        ugasen = true; 
    return false;
}

float dajNapon() {
	return voltage.read() * VCC;
}

float dajOtpor() {
    return dajNapon() * R / VCC;
}

int main() {
    bool resistance_mode = false, ugasen = true;
    //deaktivacija led dioda
    enable=1;
    
    //inicijalizacija displeja
    lcd.init();
    
    while(1) {
        char poruka[50];
        if(ocitaj_taster(ugasen))
            resistance_mode = 1 - resistance_mode;
        lcd.clear();
        if(resistance_mode) 
            sprintf(poruka, "Otpor: %f Ohm", dajOtpor());
        else 
            sprintf(poruka, "Napon: %f V", dajNapon());
        lcd.printString(poruka, 0, 0);
        wait(0.01);
    }
}