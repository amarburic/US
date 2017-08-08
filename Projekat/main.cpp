#include "mbed.h"
#define dp23 P0_0
 
#define RE(x) 2 * (x)
#define IM(x) 2 * (x) + 1

const int WINDOW_SIZE = 512;
const int BUFFER_SIZE = WINDOW_SIZE * 2;
#define SAMPLE_SPEED 0.000125f
#define PI2 6.28318530f

 
#include "stdio.h"
#include "SPI_TFT_ILI9341.h"
#include "string"
#include "Arial12x12.h"
#include "Arial24x23.h"
#include "Arial28x28.h"
#include "font_big.h"
#include <cmath>

#define VERTICAL_OFFSET 19
#define HEIGHT 240.0
#define WIDTH 320.0
#define N_LINES 5
#define LINE_HEIGHT 2
#define SEGMENT_HEIGHT ((HEIGHT - N_LINES * LINE_HEIGHT - 2 * VERTICAL_OFFSET) / (N_LINES + 1))
#define HORIZONTAL_OFFSET 10
#define N_NOTES 7
#define NOTE_RADIUS (SEGMENT_HEIGHT - 4) / 2
#define NOTE_DISTANCE (((WIDTH - 2 * HORIZONTAL_OFFSET)  - N_NOTES * (NOTE_RADIUS * 2)) / (N_NOTES + 1))
#define PI 3.141
using namespace std;

//#include "SDFileSystem.h"


//SPI_TFT_ILI9341 TFT(----,  ---,  ----,  ----, ----, ----,"TFT"); // mosi, miso, sclk, cs, reset, dc
//cs - ptd5
//rst - ptd0
//dc - ptd7
//sdi - ptd2
//sck - ptd1
//sdo - ptd3
SPI_TFT_ILI9341 TFT(PTD2, PTD3, PTD1, PTD5, PTD0, PTD7,"TFT"); // mosi, miso, sclk, cs, reset, dc

int curr_note = 1;


void clear_display() {
    TFT.cls();
    for(int i = SEGMENT_HEIGHT + VERTICAL_OFFSET, j = 0; j < 5; i += LINE_HEIGHT + SEGMENT_HEIGHT, j++)
        TFT.fillrect(0, i, WIDTH - 1, i + LINE_HEIGHT - 1, Black); 
}

void init_display() {
    TFT.claim(stdout);
    TFT.set_orientation(1);
    TFT.background(White);   
    TFT.foreground(Black);   
    TFT.cls();               
    
    clear_display();
}

void print_note(int note) {
    int first_note_y = HEIGHT - VERTICAL_OFFSET - SEGMENT_HEIGHT - LINE_HEIGHT - SEGMENT_HEIGHT / 2.0;
    TFT.fillcircle(HORIZONTAL_OFFSET + (curr_note * NOTE_DISTANCE) +  NOTE_RADIUS + (curr_note - 1) * (NOTE_RADIUS * 2), 
    first_note_y - note * (LINE_HEIGHT + SEGMENT_HEIGHT) / 2.0, NOTE_RADIUS, Black);
    curr_note++;
    if(curr_note == 8) {
        curr_note = 1;
        clear_display();
    }
}


void window(float *x, int size = BUFFER_SIZE) {
    for (int i = 0; i < size; i++) {
        float multiplier =  (float) (0.54 - 0.46 * cos( (2 * PI * i)/(size -1)));
        x[i] = multiplier * x[i];
    }
}

void four1(float data[], unsigned int nn){

    unsigned int n,mmax,m,j,istep,i;
    double wtemp,wr,wpr,wpi,wi,theta; 
    float tempr,tempi;
    
    #define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
    
    n=nn << 1;
    j=1;
    for (i=1;i<n;i+=2) { 
        if(j>i){
            SWAP(data[j],data[i]); 
            SWAP(data[j+1],data[i+1]);
        }
        m=n >> 1;
        while (m >= 2 &&j>m){
            j-=m;
            m >>= 1;
        }
        j+=m;
    }
    
    mmax=2;
    while (n > mmax) { 
        istep=mmax << 1;
        theta=(6.28318530717959/mmax); 
        wtemp=sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi=sin(theta);
        wr=1.0;
        wi=0.0;
        for (m=1;m<mmax;m+=2) { 
            for (i=m;i<=n;i+=istep) {
                j=i+mmax; 
                tempr=wr*data[j]-wi*data[j+1];
                tempi=wr*data[j+1]+wi*data[j];
                data[j]=data[i]-tempr;
                data[j+1]=data[i+1]-tempi;
                data[i] += tempr;
                data[i+1] += tempi;
            }
            wr=(wtemp=wr)*wpr-wi*wpi+wr;
            wi=wi*wpr+wtemp*wpi+wi;
        }
        mmax=istep;
    }
    
}
inline float mag2(float re, float im) {
    return re * re + im * im;
}

inline void dot(float re1, float im1, float re2, float im2,
        float *a, float *b) {
    *a = re1 * re2 - im1 * im2;
    *b = re1 * re2 + im1 * im2;
}

void hps(float *x, float *hpx, int *hpy, int size = WINDOW_SIZE) {
    int new_size = int(ceil(size / 5.0));
    
    for (int i = 0; i < new_size; i++) {
        x[RE(i)] = sqrt(mag2(x[RE(i)], x[IM(i)]) * 
                        mag2(x[RE(2 * i)], x[IM(2 * i)]) *
                        mag2(x[RE(3 * i)], x[IM(3 * i)]) *
                        mag2(x[RE(4 * i)], x[IM(4 * i)]) *
                        mag2(x[RE(5 * i)], x[IM(5 * i)]));
        x[IM(i)] = 0;
    }

    float max_mag = 0;
    int max_index = -1;
    
    for (int i = 0; i < new_size / 2; i++) {
        if (x[RE(i)] >= max_mag)
        {
            max_mag = x[RE(i)];
            max_index = i;
        }
    }

    *hpx = max_mag;
    *hpy = max_index;
   
    
}

#define FREQUENCY 8000.0

bool flag = 0;
void magic(float *sample)
{
    float mx = 0.0;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (mx < sample[i])
            mx = sample[i];
    }
    window(sample);
    
    four1(sample, WINDOW_SIZE);
    float hpx = 0.0;
    int hpy;


    // hps(sample, &hpx, &hpy);
    /*for (int i = 0; i < BUFFER_SIZE; i++) {
        float m = mag2(sample[RE(i)], sample[IM(i)]);
        if (sample[RE(i)] < 0 ? sample[RE(i)] * -1 : sample[RE(i)] > hpx) {
            hpx = sample[RE(i)] < 0 ? sample[RE(i)] * -1 : sample[RE(i)];
            hpy = i;
        }
    }*/
    for (int i = 3; i < BUFFER_SIZE; i++) {
        if (sample[i] < 0)
            sample[i] *= -1;
        if (sample[i] > hpx) {
            hpx = sample[i];
            hpy = i;
        }
    }
    float result = ((hpy * (1.0 / SAMPLE_SPEED)) / BUFFER_SIZE);
    result *= 0.373126;
    
    clear_display();
    TFT.locate(0, 0);
    printf("hpy: %d %f\n", hpy, result);
    
    
    float notes[] = { 392, 415, 440, 466, 493, 523, 554, 587 };
    float diff = 12123.0;
    int best = -1;
    for (int i = 0; i < 7; i++) {
        float x = result - notes[i];
        if (x < 0)
            x *= -1;
        if (x < diff) {
            diff = x;
            best = i;
        }
    }
    
    if (best != -1) {
        print_note(best);
        flag = 1;
    }
 
}

Ticker ticker;
float *sample;
int ind = 0, ii = 0;
AnalogIn ai(A0);
void take() {
    sample[ind] = (float)ai;
    ind++;

    if (ind >= BUFFER_SIZE - 1) {
        magic(sample);
        ind = 0;
    }
}


int main() {
    TFT.claim(stdout);
    TFT.set_font((unsigned char*) Arial28x28);
    TFT.locate(0, 0);
    sample = new float[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; i++)
        sample[i] = 0.0;

    //Inicijalizacija;
    init_display();
    //ticker.attach(&take, SAMPLE_SPEED);


    while(1) {
        if (!flag)
            take();
        wait(SAMPLE_SPEED);
    }
}
 


